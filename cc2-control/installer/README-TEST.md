# CC2 Control 1.2.19 — complete update

**FULL PRINTER REBOOT REQUIRED: this update includes the camera fix.
After installation, manually reboot the complete printer while it is idle.
Refreshing the page or restarting only CC2 Control does not activate the patch.**

Includes camera fixes from PR #183 and unreliable G-code date handling from
PR #185. Camera library bytes match the seven-hour printing observation with
AI enabled and working streaming. The new combined installer requires a complete
printer acceptance test before stable-channel publication.

Install only while connected, idle and cool (nozzle/bed at most 50 °C, targets
zero). Close live viewers first. The LAN code authorizes installation through
the panel and is not included in this package.

In the panel use Settings → Updates → upload the signed .cc2update file, or
extract the multiplatform ZIP and run the installer for your operating system.
Keep the backup path shown at completion.

Camera checks pin the executable, startup file, libraries and dependencies;
unknown configurations are refused. ai_camera is not replaced and the stream
limit is unchanged. A recognized previous personal R2 installation and its
activation preference are preserved. No automatic camera restart or reboot occurs.

After reboot, check version 1.2.19, printer connection, local and Matrix video.
For a new camera installation run:

```sh
sh /opt/usr/cc2-control/camera/manage.sh status
```

For the existing personal installation use:

```sh
sh /opt/usr/cc2-camera-recovery-r2/manage.sh status
```

Application backup restoration also restores the previous camera startup when
this update changed it, refusing unrelated later startup edits. Restore only
while idle and cool, then reboot the whole printer to change in-memory patches.

Obviously unreliable file dates are displayed as unavailable without rewriting
G-code or original timestamps. The clock still needs a reachable time source,
including on networks that block cloud access.

Thanks to tharje for camera diagnosis, reproduction and analysis, and to the
community beta testers. Specific AI detections and expired RTC sessions are not
qualified merely by observing a print with AI options enabled.
