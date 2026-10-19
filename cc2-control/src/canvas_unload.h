/* Canvas pull-back after the cut: [canvas_dev] unload_fila_dist, how far the Canvas
 * pulls a cut filament back from the toolhead (mmu.cpp extruder_retract).
 *
 * The firmware reads it once at start-up from the printer configuration followed by
 * autosave.cfg, where a later value replaces an earlier one, and its SAVE_CONFIG
 * rewrites only autosave.cfg and keeps every section in it. CC2 Control therefore
 * stores its value as a "[canvas_dev]" override in the SAVE_CONFIG block and never
 * edits the vendor configuration; removing the line returns to the configured value.
 * The firmware refuses to start with a value outside 60-350 mm, so nothing else is
 * ever written. The value applies at the next printer restart, which the page can
 * request through the same guarded reboot as the build-plate library. A saved value
 * is pending until the system has booted again (kernel boot id). */
#define CANVAS_UNLOAD_MIN 60
#define CANVAS_UNLOAD_MAX 350
#define CANVAS_UNLOAD_KEY "unload_fila_dist"
#define CANVAS_UNLOAD_CONFIG_MAX (256 * 1024)

static const char *printer_config_path = "/opt/inst/printer_dsp.cfg";
/* Absolute: the service runs from /. The marker only has to outlive a CC2 Control restart, not a reboot. */
static const char *canvas_unload_pending_path = "/tmp/cc2-control-canvas-unload-pending";
static const char *boot_id_path = "/proc/sys/kernel/random/boot_id";
static int canvas_unload_restart_requested;

/* The "#*#" lines of autosave.cfg that belong to [canvas_dev]. */
typedef struct {
    int found;                    /* the section exists */
    size_t start, end;            /* its header and key lines */
    int keys;                     /* key lines in it */
    int has_value;                /* unload_fila_dist is one of them */
    size_t value_start, value_end; /* that line, with its newline */
    double value;
} canvas_unload_entry;

static int canvas_unload_number(const char *text, size_t length, double *out) {
    char number[32];
    while (length && isspace((unsigned char)*text)) { text++; length--; }
    while (length && isspace((unsigned char)text[length - 1])) length--;
    if (!length || length >= sizeof(number)) return 0;
    memcpy(number, text, length); number[length] = 0;
    char *end; errno = 0;
    double value = strtod(number, &end);
    if (errno || *end || !isfinite(value)) return 0;
    *out = value;
    return 1;
}

/* 1 parsed (the section may be absent), -1 not a SAVE_CONFIG block this code can edit:
 * no marker, a carriage return, a duplicate section or key, or an unreadable value. */
static int canvas_unload_autosave(const char *text, size_t length, canvas_unload_entry *e) {
    static const char header[] = "#*# [canvas_dev]";
    const size_t header_len = sizeof(header) - 1, key_len = strlen(CANVAS_UNLOAD_KEY);
    memset(e, 0, sizeof(*e));
    if (memchr(text, '\r', length) || memchr(text, '\0', length)) return -1;
    const char *marker = strstr(text, AUTOSAVE_MARKER);
    if (!marker) return -1;
    for (size_t pos = (size_t)(marker - text); pos < length;) {
        size_t eol = autosave_line_end(text, length, pos), next = eol < length ? eol + 1 : length;
        if (eol - pos != header_len || memcmp(text + pos, header, header_len)) { pos = next; continue; }
        if (e->found) return -1;
        e->found = 1; e->start = pos;
        size_t line = next;
        while (line < length) {
            size_t line_eol = autosave_line_end(text, length, line), after = line_eol < length ? line_eol + 1 : length;
            if (line_eol - line < 5 || memcmp(text + line, "#*# ", 4) || text[line + 4] == '[') break;
            e->keys++;
            const char *key = text + line + 4, *sep = memchr(key, '=', line_eol - line - 4);
            if (!sep) sep = memchr(key, ':', line_eol - line - 4);
            size_t name_len = sep ? (size_t)(sep - key) : 0;
            while (name_len && isspace((unsigned char)key[name_len - 1])) name_len--;
            if (name_len == key_len && !memcmp(key, CANVAS_UNLOAD_KEY, key_len)) {
                if (e->has_value || !canvas_unload_number(sep + 1, (size_t)(text + line_eol - sep - 1), &e->value))
                    return -1;
                e->has_value = 1; e->value_start = line; e->value_end = after;
            }
            line = after;
        }
        e->end = line;
        pos = line;
    }
    return 1;
}

/* The value the vendor configuration sets, without autosave.cfg: the last
 * unload_fila_dist of the last [canvas_dev] section, as the firmware's INI loader reads
 * it ('#' starts a comment anywhere on a line). 1 found, 0 absent or unreadable. */
static int canvas_unload_configured(const char *text, size_t length, double *out) {
    const size_t key_len = strlen(CANVAS_UNLOAD_KEY);
    int inside = 0, found = 0;
    for (size_t pos = 0; pos < length;) {
        size_t eol = autosave_line_end(text, length, pos), next = eol < length ? eol + 1 : length;
        const char *line = text + pos, *hash = memchr(line, '#', eol - pos);
        size_t n = hash ? (size_t)(hash - line) : eol - pos;
        while (n && isspace((unsigned char)*line)) { line++; n--; }
        while (n && isspace((unsigned char)line[n - 1])) n--;
        if (n && line[0] == '[') {
            inside = n == 12 && !memcmp(line, "[canvas_dev]", 12);
        } else if (inside && n > key_len && !memcmp(line, CANVAS_UNLOAD_KEY, key_len)) {
            size_t rest = key_len;
            while (rest < n && isspace((unsigned char)line[rest])) rest++;
            double value;
            if (rest < n && (line[rest] == '=' || line[rest] == ':') &&
                canvas_unload_number(line + rest + 1, n - rest - 1, &value)) { *out = value; found = 1; }
        }
        pos = next;
    }
    return found;
}

/* A copy of autosave.cfg with the override set to `value`, or removed when `value` is 0,
 * checked by parsing it back. Every byte outside the edited lines stays as it was. */
static char *canvas_unload_rewrite(const char *text, size_t length, int value, size_t *out_length) {
    static const char separator[] = "#*#\n";
    canvas_unload_entry e, check;
    if (canvas_unload_autosave(text, length, &e) != 1) return NULL;
    char line[64];
    int line_len = value ? snprintf(line, sizeof(line), "#*# " CANVAS_UNLOAD_KEY " = %d\n", value) : 0;
    size_t cut_start, cut_end; /* replaced by `insert` */
    char insert[128]; size_t insert_len = 0;
    if (value && e.has_value) {
        cut_start = e.value_start; cut_end = e.value_end;
        memcpy(insert, line, (size_t)line_len); insert_len = (size_t)line_len;
        if (cut_end == length && text[length - 1] != '\n') insert_len--; /* the file ended on that line */
    } else if (value && e.found) {
        size_t header_end = autosave_line_end(text, length, e.start);
        if (header_end == length) return NULL; /* a header without a newline: not ours to repair */
        cut_start = cut_end = header_end + 1;
        memcpy(insert, line, (size_t)line_len); insert_len = (size_t)line_len;
    } else if (value) { /* a new section after the last non-empty line, like SAVE_CONFIG lays them out */
        cut_start = cut_end = length;
        while (cut_start && text[cut_start - 1] == '\n') cut_start = --cut_end;
        insert_len = (size_t)snprintf(insert, sizeof(insert), "\n%s%s#*# [canvas_dev]\n%.*s", separator, separator,
                                      line_len - 1, line);
        if (cut_end == length) insert[insert_len++] = '\n';
    } else if (!e.has_value) {
        cut_start = cut_end = 0; /* nothing to remove */
    } else if (e.keys > 1) {
        cut_start = e.value_start; cut_end = e.value_end;
    } else { /* the whole section, with the separator lines before it */
        cut_start = e.start; cut_end = e.end;
        while (cut_start >= 4 && !memcmp(text + cut_start - 4, separator, 4) &&
               (cut_start == 4 || text[cut_start - 5] == '\n'))
            cut_start -= 4;
    }
    size_t used = cut_start + insert_len + (length - cut_end);
    char *copy = malloc(used + 1);
    if (!copy) return NULL;
    memcpy(copy, text, cut_start);
    memcpy(copy + cut_start, insert, insert_len);
    memcpy(copy + cut_start + insert_len, text + cut_end, length - cut_end);
    copy[used] = 0;
    if (canvas_unload_autosave(copy, used, &check) != 1 || check.has_value != (value != 0) ||
        (value && check.value != value)) {
        free(copy); return NULL;
    }
    *out_length = used;
    return copy;
}

static int canvas_unload_boot_id(char *out, size_t cap) {
    size_t length = 0;
    char *text = plates_read_file(boot_id_path, 64, &length);
    if (!text) return 0;
    while (length && isspace((unsigned char)text[length - 1])) length--;
    int ok = length && length < cap;
    if (ok) { memcpy(out, text, length); out[length] = 0; }
    free(text);
    return ok;
}

/* Saved since this boot: the pending file holds the boot id it was written in. A file
 * from an earlier boot is stale, so it goes. */
static int canvas_unload_pending(void) {
    char now[64], saved[64]; size_t length = 0;
    char *text = plates_read_file(canvas_unload_pending_path, sizeof(saved) - 1, &length);
    if (!text) return 0;
    snprintf(saved, sizeof(saved), "%.*s", (int)length, text);
    free(text);
    if (canvas_unload_boot_id(now, sizeof(now)) && !strcmp(now, saved)) return 1;
    unlink(canvas_unload_pending_path);
    return 0;
}

static void canvas_unload_number_json(json_builder *b, const char *name, int known, double value) {
    if (known) json_builder_printf(b, ",\"%s\":%.10g", name, value);
    else json_builder_printf(b, ",\"%s\":null", name);
}

static void canvas_unload_state(int fd, int status) {
    size_t length = 0;
    double configured = 0;
    canvas_unload_entry e = {0};
    char *text = plates_read_file(printer_config_path, CANVAS_UNLOAD_CONFIG_MAX, &length);
    int have_configured = text && canvas_unload_configured(text, length, &configured);
    free(text);
    text = plates_read_file(printer_autosave_path, AUTOSAVE_FILE_MAX, &length);
    int editable = text && canvas_unload_autosave(text, length, &e) == 1;
    free(text);
    const char *restart = "none";
    if (canvas_unload_restart_requested) {
        if (reboot_pending) restart = "restarting";
        else if (strcmp(reboot_error, "none")) { restart = reboot_error; canvas_unload_restart_requested = 0; }
    }
    char body[512];
    json_builder b = {body, 0, sizeof(body), 0};
    json_builder_printf(&b, "{\"ok\":true,\"editable\":%s,\"min\":%d,\"max\":%d", editable ? "true" : "false",
                        CANVAS_UNLOAD_MIN, CANVAS_UNLOAD_MAX);
    canvas_unload_number_json(&b, "configured", have_configured, configured);
    canvas_unload_number_json(&b, "override", editable && e.has_value, editable ? e.value : 0);
    json_builder_printf(&b, ",\"pending_restart\":%s,\"restart\":\"%s\"}\n",
                        canvas_unload_pending() ? "true" : "false", restart);
    plates_reply(fd, b.failed ? 500 : status, b.failed ? "{\"ok\":false,\"error\":\"Reply too large\"}\n" : body);
}

static void canvas_unload_response(int fd) { canvas_unload_state(fd, 200); }

/* Body: a whole number of millimetres, or "default" to remove the override. */
static void canvas_unload_save_response(int fd, const mqtt_client *mqtt, const char *body, size_t body_len) {
    while (body_len && isspace((unsigned char)*body)) { body++; body_len--; }
    while (body_len && isspace((unsigned char)body[body_len - 1])) body_len--;
    int value = -1;
    if (body_len == 7 && !memcmp(body, "default", 7)) value = 0;
    else if (body_len >= 2 && body_len <= 3) {
        value = 0;
        for (size_t i = 0; i < body_len; ++i) value = isdigit((unsigned char)body[i]) ? value * 10 + body[i] - '0' : -1000;
        if (value < CANVAS_UNLOAD_MIN || value > CANVAS_UNLOAD_MAX) value = -1;
    }
    if (value < 0) { plates_fail(fd, 400, "Enter a whole number of millimetres from 60 to 350"); return; }
    const char *busy = plates_printer_ready(mqtt);
    if (busy) { plates_fail(fd, 409, busy); return; }
    size_t length = 0, new_length = 0, again_length = 0;
    char *file = plates_read_file(printer_autosave_path, AUTOSAVE_FILE_MAX, &length);
    if (!file) { plates_fail(fd, 500, "Cannot read the printer configuration"); return; }
    char *updated = canvas_unload_rewrite(file, length, value, &new_length);
    if (!updated) {
        free(file);
        plates_fail(fd, 409, "The printer's autosave.cfg has a layout CC2 Control does not edit");
        return;
    }
    if (new_length == length && !memcmp(updated, file, length)) { /* already so */
        free(file); free(updated); canvas_unload_state(fd, 200); return;
    }
    char backup[PATH_MAX_LOCAL], boot[64];
    /* The firmware may have saved its configuration since the file was read. */
    char *again = plates_read_file(printer_autosave_path, AUTOSAVE_FILE_MAX, &again_length);
    int unchanged = again && again_length == length && !memcmp(again, file, length);
    free(again); free(file);
    if (!unchanged) { free(updated); plates_fail(fd, 409, "The printer changed its configuration meanwhile; try again"); return; }
    if (!autosave_backup_path(backup, sizeof(backup)) ||
        plates_replace_file(printer_autosave_path, backup, updated, new_length, autosave_mode()) != 0) {
        free(updated); plates_fail(fd, 500, "Cannot write the printer configuration"); return;
    }
    free(updated);
    if (!canvas_unload_boot_id(boot, sizeof(boot)) ||
        plates_replace_file(canvas_unload_pending_path, NULL, boot, strlen(boot), 0644) != 0)
        fprintf(stderr, "Canvas pull-back: saved, but cannot record that a restart is pending\n");
    canvas_unload_state(fd, 200);
}

/* Body: RESTART. Reboots the printer to apply a saved value, once it is still idle. */
static void canvas_unload_restart_response(int fd, mqtt_client *mqtt, const char *body, size_t body_len) {
    while (body_len && isspace((unsigned char)body[body_len - 1])) body_len--;
    if (body_len != 7 || memcmp(body, "RESTART", 7)) { plates_fail(fd, 400, "Explicit restart confirmation required"); return; }
    if (!canvas_unload_pending()) { plates_fail(fd, 409, "No saved pull-back distance is waiting for a restart"); return; }
    const char *busy = plates_printer_ready(mqtt);
    if (busy) { plates_fail(fd, 409, busy); return; }
    canvas_unload_restart_requested = 1; plates_mqtt = mqtt;
    reboot_guard = plates_reboot_guard;
    reboot_pending = 1; reboot_launched = 0; reboot_due = recovery_clock() + 2; reboot_error = "none";
    plates_reply(fd, 202, "{\"ok\":true,\"restarting\":true}\n");
}
