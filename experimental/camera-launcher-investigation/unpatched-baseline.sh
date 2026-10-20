#!/bin/sh
# Controlled comparison only: restart the original camera, never the printer service.
set -eu
umask 077
export LD_LIBRARY_PATH=/opt/lib
WORK=/tmp/cc2-camera-baseline
QUALIFIED=ec5633b17de329d5b18c3f8d8de6ebf389391a04193536d976ba167022d07b99
camera_pid() {
    PID=$(pidof ai_camera 2>/dev/null || true)
    case "$PID" in ''|*' '*|*[!0-9]*) echo 'STOP: exactly one camera process is required.' >&2; return 1;; esac
}
qualified() {
    [ "$(readlink "/proc/$PID/exe")" = /opt/bin/ai_camera ] || { echo 'STOP: unexpected camera path.' >&2; return 1; }
    for FILE in /opt/bin/ai_camera "/proc/$PID/exe"; do
        [ "$(sha256sum "$FILE" | awk '{print $1}')" = "$QUALIFIED" ] || { echo 'STOP: unqualified camera executable.' >&2; return 1; }
    done
    if grep -F 'libcc2-camera-reap' "/proc/$PID/maps" >/dev/null 2>&1; then
        echo 'STOP: trial library is active; reboot the idle printer normally before this comparison.' >&2
        return 1
    fi
}
idle() {
    STATE=$(wget -qO- http://127.0.0.1:8081/api/printer)
    case "$STATE" in *'"connected":true'*'"machine":{"status":1,'*) ;; *) echo 'STOP: connected Idle printer required.' >&2; return 1;; esac
    AGE=$(printf '%s' "$STATE" | sed -n 's/.*"last_message_age":\([0-9][0-9]*\),.*/\1/p')
    case "$AGE" in ''|*[!0-9]*) echo 'STOP: missing telemetry age.' >&2; return 1;; esac
    [ "$AGE" -le 15 ] || { echo 'STOP: stale printer state.' >&2; return 1; }
}
dependencies() {
    [ -x /lib/ld-linux-armhf.so.3 ] || { echo 'STOP: ARM loader unavailable.' >&2; return 1; }
    if ! LD_PRELOAD= CC2_CAMERA_REAP_TRIAL=0 /lib/ld-linux-armhf.so.3 --list /opt/bin/ai_camera > "$WORK/loader-check.txt" 2>&1; then
        echo 'STOP: unresolved camera dependencies; no process stopped.' >&2
        return 1
    fi
    if grep -q 'not found' "$WORK/loader-check.txt"; then
        echo 'STOP: missing camera dependency; no process stopped.' >&2
        return 1
    fi
}
check() {
    camera_pid
    qualified
    idle
    mkdir -p "$WORK"
    dependencies
    CAMERA_CWD=$(readlink "/proc/$PID/cwd")
    [ -d "$CAMERA_CWD" ] || { echo 'STOP: original camera working directory unavailable.' >&2; return 1; }
}
status() {
    camera_pid
    qualified
    echo "Original camera PID: $PID (no trial library mapped)"
    grep -E '^(State:|VmSize:|VmRSS:|Threads:)' "/proc/$PID/status"
}
case "${1:-check}" in
check)
    check
    status
    echo 'CHECK PASS: original camera, dependencies and fresh Idle state verified. No process restarted.'
    ;;
start)
    check
    echo 'Close local video viewers first. Confirm Matrix video works before this comparison.'
    echo 'This restarts ONLY the original camera without R2. Matrix/local video may stop until an idle printer reboot.'
    printf 'Type RESTART CAMERA to proceed, anything else to cancel: '
    ANSWER=''
    IFS= read -r ANSWER || true
    [ "$ANSWER" = 'RESTART CAMERA' ] || { echo 'Cancelled. No process restarted.'; exit 0; }
    check
    OLD_PID=$PID
    status > "$WORK/before.txt"
    # Recheck the selected PID immediately before signalling it; never use killall.
    camera_pid
    [ "$PID" = "$OLD_PID" ] || { echo 'STOP: camera process changed.' >&2; exit 1; }
    qualified
    kill -TERM "$PID"
    COUNT=0
    while kill -0 "$OLD_PID" 2>/dev/null; do
        [ "$COUNT" -lt 10 ] || { echo 'STOP: original camera did not exit; no forced kill. Reboot only while idle.' >&2; exit 1; }
        COUNT=$((COUNT+1))
        sleep 1
    done
    sleep 1
    if pidof ai_camera >/dev/null 2>&1; then echo 'STOP: another camera process appeared; no duplicate started.' >&2; exit 1; fi
    cd "$CAMERA_CWD"
    LD_PRELOAD= CC2_CAMERA_REAP_TRIAL=0 CC2_CAMERA_REAP_ENABLE_LAN=0 /opt/bin/ai_camera < /dev/null > "$WORK/ai_camera-original.log" 2>&1 &
    NEW_PID=$!
    sleep 5
    kill -0 "$NEW_PID" 2>/dev/null || { echo 'STOP: original camera exited. Reboot the idle printer normally.' >&2; exit 1; }
    camera_pid
    [ "$PID" = "$NEW_PID" ] || { echo 'STOP: unexpected camera PID. Reboot only while idle.' >&2; exit 1; }
    status
    echo 'ORIGINAL CAMERA RESTARTED: no trial library loaded. Video/Matrix are not confirmed by this message.'
    echo 'Keep SSH open. Wait 30 seconds, close/reopen Matrix once and report video plus printer online/offline state.'
    echo 'Do not start a print. If video fails, save status then reboot the idle printer normally for recovery.'
    echo 'Do not share the raw camera log: it may contain RTC tokens.'
    ;;
status) status ;;
*) echo 'Usage: sh unpatched-baseline.sh check | start | status' >&2; exit 2 ;;
esac
