# CC2 Control application update — Community Firmware V4.4 only

The maintainer confirmed installation, signed manual and online panel updates,
restart, connection, Canvas and configuration retention. The cleared Canvas
analysis notification fix was also confirmed on the printer.

Extract the complete installer ZIP on your computer. At connected Idle with
nozzle/bed <=50 C and both targets zero, run Install-Windows.cmd or
sh ./install-cc2-control.sh PRINTER-IP. Use root and your SSH password.
Keep the backup/restore instructions printed by your installer; do not factory-reset.
Reload with Ctrl+F5 and check the version shown in the release and /api/health.

For subsequent upgrades use Settings > Updates: Check, LAN code, Download and
verify, then Install with a separate confirmation. Signed local .cc2update
uploads are also supported; ZIP installer files are not panel updates.
No reinstallation is needed when you already run the release version.

Full guide: https://github.com/Neme77/centauri-carbon-2-community/blob/develop/docs/UPDATE_CC2.md
Italian: https://github.com/Neme77/centauri-carbon-2-community/blob/develop/docs/UPDATE_CC2.it.md

Firmware/callback remain unchanged; Canvas stays on MQTT. Forced-failure rollback
is tested on the host but has not been deliberately exercised on the printer.
