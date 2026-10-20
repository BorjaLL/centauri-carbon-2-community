# Camera launcher and Matrix investigation — candidate only

This is a launcher-only candidate for the unchanged R2 library. It is not a new tested camera runtime or a replacement beta package.

Changes: explicitly set LD_LIBRARY_PATH=/opt/lib; resolve camera dependencies with the exact ARM loader before stopping the existing process; disconnect stdin from the background camera process. Do not source /etc/profile or depend on nohup.

`camera-readonly.sh` reports process memory and bounded event counts. It sends no camera commands, restarts no service, and avoids printing camera log lines that could contain RTC tokens. Counts over the last 400 lines may include older processes and do not prove present RTC state.

The reported R2 local stress test survived 450 open/close connections with stable virtual memory and 20 fps. Matrix has a separate reported regression after activation. It remains unknown whether that is caused by restarting ai_camera mid-session or by the trial. A controlled unpatched restart comparison is the next investigation; do not assume that boot activation is already validated.

No persistent startup changes, printer restart, heating or motion is performed by this repository preparation. Do not deploy trial.sh alone over an existing package: its manifest must be regenerated and the complete candidate validated before distribution.

Credit: tharje and dwolfuk for the detailed local/Matrix testing.
