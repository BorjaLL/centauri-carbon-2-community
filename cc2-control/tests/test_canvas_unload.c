#define main cc2_main_original
#include "../src/main.c"
#undef main
#include <assert.h>

/* The firmware's own layout, as SAVE_CONFIG writes it. */
static const char AUTOSAVE[] =
    "#*# <---------------------- SAVE_CONFIG ---------------------->\n"
    "#*# DO NOT EDIT THIS BLOCK OR BELOW. The contents are auto-generated.\n"
    "#*#\n"
    "#*# [input_shaper]\n"
    "#*# shaper_type_x = zv\n"
    "#*# shaper_freq_x = 52.600000\n"
    "#*#\n"
    "#*#\n"
    "#*# [stepper_z]\n"
    "#*# position_endstop = 5.539131\n";

static const char VENDOR[] =
    "[extruder]\n"
    "unload_fila_dist=10\n"
    "[canvas_dev]\n"
    "serial=/dev/ttyS1\n"
    "unload_fila_dist_2=5\n"
    "unload_fila_dist=80    # 退料距离 范围(60-350）\n"
    "  unload_fila_dist : 85 # the last one wins\n"
    "unload_fila_time=30#1\n"
    "[stepper_x]\n"
    "unload_fila_dist=20\n";

static char response[4096];
static int status_of(const char *text) { int status = 0; sscanf(text, "HTTP/1.1 %d", &status); return status; }

static void write_file(const char *path, const char *text) {
    FILE *f = fopen(path, "wb"); assert(f);
    assert(fwrite(text, 1, strlen(text), f) == strlen(text)); fclose(f);
}

static char *read_all(const char *path) {
    size_t length = 0; char *text = plates_read_file(path, AUTOSAVE_FILE_MAX, &length);
    assert(text); return text;
}

#define GET 0
#define SAVE 1
#define RESTART 2
static int call(int handler, mqtt_client *mqtt, const char *body) {
    int pair[2]; assert(!socketpair(AF_UNIX, SOCK_STREAM, 0, pair));
    size_t length = body ? strlen(body) : 0;
    if (handler == GET) canvas_unload_response(pair[0]);
    else if (handler == SAVE) canvas_unload_save_response(pair[0], mqtt, body, length);
    else canvas_unload_restart_response(pair[0], mqtt, body, length);
    close(pair[0]);
    size_t used = 0; ssize_t n;
    while ((n = recv(pair[1], response + used, sizeof(response) - 1 - used, 0)) > 0) used += (size_t)n;
    response[used] = 0; close(pair[1]);
    return status_of(response);
}

static void fresh(mqtt_client *mqtt) {
    mqtt->connected = mqtt->registered = 1; mqtt->have_machine_status = 1; mqtt->machine_status = 1;
    mqtt->last_message = time(NULL);
    telemetry.fd = 0; telemetry.ready = 1; clock_gettime(CLOCK_MONOTONIC, &telemetry.last_rx);
}

static pid_t refuse_launch(void) { assert(!"the guard must stop this reboot"); return -1; }

static char *rewrite(const char *text, int value) {
    size_t length = 0; char *copy = canvas_unload_rewrite(text, strlen(text), value, &length);
    if (copy) assert(strlen(copy) == length);
    return copy;
}

static void test_vendor_value(void) {
    double value = 0;
    assert(canvas_unload_configured(VENDOR, strlen(VENDOR), &value) && value == 85);
    const char none[] = "[canvas_dev]\nunload_fila_dist_2=5\n# unload_fila_dist=90\n[x]\nunload_fila_dist=91\n";
    assert(!canvas_unload_configured(none, strlen(none), &value));
    const char bad[] = "[canvas_dev]\nunload_fila_dist=far\n";
    assert(!canvas_unload_configured(bad, strlen(bad), &value));
}

static void test_rewrite(void) {
    /* A new section goes after the last one, separated the way SAVE_CONFIG separates them. */
    char *added = rewrite(AUTOSAVE, 300);
    assert(added && !strcmp(added, "#*# <---------------------- SAVE_CONFIG ---------------------->\n"
                                   "#*# DO NOT EDIT THIS BLOCK OR BELOW. The contents are auto-generated.\n"
                                   "#*#\n#*# [input_shaper]\n#*# shaper_type_x = zv\n#*# shaper_freq_x = 52.600000\n"
                                   "#*#\n#*#\n#*# [stepper_z]\n#*# position_endstop = 5.539131\n"
                                   "#*#\n#*#\n#*# [canvas_dev]\n#*# unload_fila_dist = 300\n"));
    /* Replacing changes only that line; removing restores the original bytes. */
    char *changed = rewrite(added, 120);
    assert(changed && strstr(changed, "#*# unload_fila_dist = 120\n") && !strstr(changed, "= 300") &&
           strlen(changed) == strlen(added));
    char *removed = rewrite(changed, 0);
    assert(removed && !strcmp(removed, AUTOSAVE));
    free(added); free(changed); free(removed);

    /* A section in the middle, and one holding other keys, which stay. */
    const char middle[] =
        "#*# <---------------------- SAVE_CONFIG ---------------------->\n#*#\n"
        "#*# [canvas_dev]\n#*# unload_fila_dist = 200\n#*#\n#*#\n#*# [stepper_z]\n#*# position_endstop = 5\n";
    removed = rewrite(middle, 0);
    assert(removed && !strcmp(removed, "#*# <---------------------- SAVE_CONFIG ---------------------->\n"
                                       "#*#\n#*#\n#*# [stepper_z]\n#*# position_endstop = 5\n"));
    free(removed);
    const char shared[] =
        "#*# <---------------------- SAVE_CONFIG ---------------------->\n"
        "#*# [canvas_dev]\n#*# load_fila_dist = 900\n";
    added = rewrite(shared, 350);
    assert(added && !strcmp(added, "#*# <---------------------- SAVE_CONFIG ---------------------->\n"
                                   "#*# [canvas_dev]\n#*# unload_fila_dist = 350\n#*# load_fila_dist = 900\n"));
    removed = rewrite(added, 0);
    assert(removed && !strcmp(removed, shared));
    free(added); free(removed);

    /* A file that ends without a newline keeps doing so on that line. */
    const char tail[] = "#*# <---------------------- SAVE_CONFIG ---------------------->\n"
                        "#*# [canvas_dev]\n#*# unload_fila_dist = 61";
    changed = rewrite(tail, 62);
    assert(changed && !strcmp(changed, "#*# <---------------------- SAVE_CONFIG ---------------------->\n"
                                       "#*# [canvas_dev]\n#*# unload_fila_dist = 62"));
    free(changed);
    const char short_tail[] = "#*# <---------------------- SAVE_CONFIG ---------------------->";
    added = rewrite(short_tail, 70);
    assert(added && !strcmp(added, "#*# <---------------------- SAVE_CONFIG ---------------------->\n"
                                   "#*#\n#*#\n#*# [canvas_dev]\n#*# unload_fila_dist = 70\n"));
    free(added);

    /* Removing what is not there changes nothing. */
    removed = rewrite(AUTOSAVE, 0);
    assert(removed && !strcmp(removed, AUTOSAVE));
    free(removed);

    /* Files this code does not edit. */
    assert(!rewrite("[printer]\nkinematics = corexy\n", 100));
    assert(!rewrite("#*# <---------------------- SAVE_CONFIG ---------------------->\r\n#*# [x]\r\n", 100));
    assert(!rewrite("#*# <---------------------- SAVE_CONFIG ---------------------->\n"
                    "#*# [canvas_dev]\n#*# unload_fila_dist = 100\n#*# [canvas_dev]\n#*# unload_fila_dist = 90\n", 120));
    assert(!rewrite("#*# <---------------------- SAVE_CONFIG ---------------------->\n"
                    "#*# [canvas_dev]\n#*# unload_fila_dist = 100\n#*# unload_fila_dist = 90\n", 120));
    assert(!rewrite("#*# <---------------------- SAVE_CONFIG ---------------------->\n"
                    "#*# [canvas_dev]\n#*# unload_fila_dist = lots\n", 120));
    /* A section before the marker is the printer configuration's, not the SAVE_CONFIG block's. */
    added = rewrite("[canvas_dev]\nunload_fila_dist = 80\n#*# <---------------------- SAVE_CONFIG ---------------------->\n", 90);
    assert(added && strstr(added, "[canvas_dev]\nunload_fila_dist = 80\n") && strstr(added, "#*# unload_fila_dist = 90\n"));
    free(added);
}

static void test_http(void) {
    mqtt_client mqtt; memset(&mqtt, 0, sizeof(mqtt)); fresh(&mqtt);
    printer_autosave_path = "autosave.cfg"; printer_config_path = "printer_dsp.cfg";
    canvas_unload_pending_path = "canvas-unload-pending"; boot_id_path = "boot_id";
    write_file("autosave.cfg", AUTOSAVE); write_file("printer_dsp.cfg", VENDOR); write_file("boot_id", "boot-one\n");
    unlink("autosave_backup.cfg"); unlink("canvas-unload-pending");

    assert(call(GET, &mqtt, NULL) == 200 && strstr(response, "\"editable\":true") &&
           strstr(response, "\"min\":60,\"max\":350") && strstr(response, "\"configured\":85") &&
           strstr(response, "\"override\":null") && strstr(response, "\"pending_restart\":false") &&
           strstr(response, "\"restart\":\"none\""));

    const char *refused[] = {"", "59", "351", "1000", "abc", "1e2", "-80", "80.5", "0", "default ", "Default"};
    for (size_t i = 0; i < sizeof(refused) / sizeof(*refused); ++i) {
        int status = call(SAVE, &mqtt, refused[i]);
        assert(status == 400 || (status == 200 && !strcmp(refused[i], "default ")));
    }
    char *unchanged = read_all("autosave.cfg"); assert(!strcmp(unchanged, AUTOSAVE)); free(unchanged);

    mqtt.machine_status = 2; assert(call(SAVE, &mqtt, "300") == 409 && strstr(response, "idle"));
    fresh(&mqtt); mqtt.last_message = time(NULL) - 60; assert(call(SAVE, &mqtt, "300") == 409);
    fresh(&mqtt); assert(call(RESTART, &mqtt, "RESTART") == 409 && strstr(response, "waiting"));

    assert(call(SAVE, &mqtt, " 300\n") == 200 && strstr(response, "\"override\":300") &&
           strstr(response, "\"configured\":85") && strstr(response, "\"pending_restart\":true"));
    char *saved = read_all("autosave.cfg"), *backup = read_all("autosave_backup.cfg");
    assert(strstr(saved, "#*# [canvas_dev]\n#*# unload_fila_dist = 300\n") && !strcmp(backup, AUTOSAVE));
    free(saved); free(backup);
    assert(call(SAVE, &mqtt, "300") == 200 && strstr(response, "\"override\":300")); /* nothing to change */

    /* A layout it does not edit is reported and left alone. */
    write_file("autosave.cfg", "[printer]\n");
    assert(call(GET, &mqtt, NULL) == 200 && strstr(response, "\"editable\":false") && strstr(response, "\"override\":null"));
    assert(call(SAVE, &mqtt, "200") == 409);
    char *foreign = read_all("autosave.cfg"); assert(!strcmp(foreign, "[printer]\n")); free(foreign);
    write_file("autosave.cfg", AUTOSAVE);
    assert(call(SAVE, &mqtt, "300") == 200);

    /* The restart needs its confirmation and an idle printer, then waits for the reboot guard. */
    assert(call(RESTART, &mqtt, "restart") == 400 && !reboot_pending);
    mqtt.machine_status = 2; assert(call(RESTART, &mqtt, "RESTART") == 409 && !reboot_pending);
    fresh(&mqtt);
    assert(call(RESTART, &mqtt, "RESTART\n") == 202 && reboot_pending && reboot_guard == plates_reboot_guard);
    assert(call(GET, &mqtt, NULL) == 200 && strstr(response, "\"restart\":\"restarting\""));
    assert(call(SAVE, &mqtt, "200") == 409 && strstr(response, "restart is already pending"));
    /* A print started meanwhile stops the reboot; the page learns why, once. */
    reboot_launcher = refuse_launch; mqtt.machine_status = 2; reboot_due = recovery_clock() - 1;
    recovery_tick();
    assert(!reboot_pending && !strcmp(reboot_error, "printer_busy"));
    assert(call(GET, &mqtt, NULL) == 200 && strstr(response, "\"restart\":\"printer_busy\"") &&
           strstr(response, "\"pending_restart\":true"));
    assert(call(GET, &mqtt, NULL) == 200 && strstr(response, "\"restart\":\"none\""));
    reboot_guard = NULL; reboot_error = "none"; fresh(&mqtt);

    /* After the next boot the saved value is the one in effect. */
    write_file("boot_id", "boot-two\n");
    assert(call(GET, &mqtt, NULL) == 200 && strstr(response, "\"pending_restart\":false") &&
           strstr(response, "\"override\":300") && access("canvas-unload-pending", F_OK) != 0);

    /* Default removes the override and restores the file. */
    assert(call(SAVE, &mqtt, "default") == 200 && strstr(response, "\"override\":null") &&
           strstr(response, "\"pending_restart\":true"));
    char *restored = read_all("autosave.cfg"); assert(!strcmp(restored, AUTOSAVE)); free(restored);

    /* An unreadable configuration is reported as unknown. */
    unlink("printer_dsp.cfg");
    assert(call(GET, &mqtt, NULL) == 200 && strstr(response, "\"configured\":null"));
}

int main(void) {
    test_vendor_value();
    test_rewrite();
    test_http();
    puts("PASS: Canvas pull-back override: vendor value, SAVE_CONFIG-layout edits and exact removal, refused layouts, "
         "range/idle checks, backup, pending restart by boot id, guarded reboot");
    return 0;
}
