#!/bin/sh
# Metadata only: never print camera logs, RTC tokens, printer identity or LAN codes.
set -eu
umask 077
date
PIDS=$(pidof ai_camera 2>/dev/null || true)
case "$PIDS" in
    ''|*' '*) echo 'STOP: exactly one camera process is required.'; exit 1 ;;
    *[!0-9]*) echo 'STOP: invalid camera PID.'; exit 1 ;;
esac
PID=$PIDS
echo "Camera PID: $PID"
sha256sum /opt/bin/ai_camera "/proc/$PID/exe"
grep -E '^(State:|VmSize:|VmRSS:|Threads:)' "/proc/$PID/status"
if grep -F 'libcc2-camera-reap-r2.so' "/proc/$PID/maps" >/dev/null 2>&1; then
    echo 'R2 library mapped: yes'
else
    echo 'R2 library mapped: no'
fi
if [ -r /tmp/cc2-camera-reap.status ]; then
    echo 'Marker (may belong to an older process; compare PID):'
    cat /tmp/cc2-camera-reap.status
fi
for LOG in /opt/usr/logs/ai_camera.log /tmp/cc2-camera-reap-r2/ai_camera-trial.log; do
    if [ -r "$LOG" ]; then
        echo "Event counts in last 400 lines of $LOG (may include earlier processes; not proof of current RTC state):"
        tail -n 400 "$LOG" | awk '
            /local_video_monitor/ { local_monitor++ }
            /RtcMonitorStart/ { rtc_start++ }
            /New client connected/ { clients++ }
            /handle_client start/ { handlers++ }
            /client exit/ { exits++ }
            /Resource temporarily unavailable/ { eagain++ }
            END {
                printf "local_video_monitor=%d RtcMonitorStart=%d accepts=%d handlers=%d exits=%d EAGAIN=%d\n", local_monitor,rtc_start,clients,handlers,exits,eagain
            }'
    fi
done
echo 'Read-only capture complete. No process restarted and no camera command sent.'
