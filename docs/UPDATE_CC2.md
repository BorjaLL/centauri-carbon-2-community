# CC2 Control 1.2.5 update — Community Firmware V4.4 only

[Italiano](UPDATE_CC2.it.md) · [Homepage](../README.md)

Includes Hungarian (Magyar) by [pezso92](https://github.com/pezso92), and advanced temperature/nozzle meshes with soaked calibration by [beanbo](https://github.com/beanbo).

**Only an existing, configured Community Firmware V4.4 installation is supported. Do not install on V4.3, original ELEGOO firmware or another printer model.** Vendor base 02.01.01.07 alone is insufficient: the installer checks the exact community printer executable on disk and in the running process, plus the update daemon, before stopping the service. Unknown fingerprints are refused.

This ZIP updates the application over SSH. It does not flash firmware, install SSH or replace the callback module. Canvas stays on MQTT. LAN/WAN, lights and the existing validated features remain included.

## 1. Prepare

1. Download **CC2-Control-1.2.5-Multiplatform-Update.zip** and its checksum from the [application release](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/CC2-Control-1.2.5-V4.4).
2. Verify the ZIP SHA-256 against the release checksum:

~~~powershell
Get-FileHash -Algorithm SHA256 -LiteralPath '.\CC2-Control-1.2.5-Multiplatform-Update.zip'
~~~

3. Confirm V4.4 is installed and CC2 Control reports **Printer connected**.
4. Wait for **Idle**, with no print or calibration running, nozzle and bed at most **50 °C**, and both targets **0 °C**. Close live viewers and other control pages during installation.
5. Note the printer IP. The computer needs local network access and OpenSSH (ssh and scp).
6. Extract the entire ZIP into a new folder. Do not run launchers inside the compressed ZIP or use this ZIP in the printer USB firmware-update menu.

LAN credentials, material presets, appearance, plate/nozzle data and spool inventory are preserved. An automatic backup records the previous application and service launcher.

## 2. Install

### Windows

Open the extracted folder and double-click **Install-Windows.cmd**; enter the printer IP.
Alternatively, open PowerShell in that folder:

~~~powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\Install-CC2-Control.ps1 -PrinterIp 192.168.1.50
~~~

Replace the example IP. Windows OpenSSH Client must be installed.

### Linux and macOS

Open a terminal in the extracted folder:

~~~sh
sh ./install-cc2-control.sh 192.168.1.50
~~~

Replace the example IP.

### SSH and completion

Username **root**, default password **MTY4ODE2**, or your replacement if changed. Password entry is invisible and may be requested more than once. This is separate from the LAN access code.

Verify the printer host-key fingerprint before accepting a first SSH connection. For an expected host-key change only, follow [host-key recovery](../cc2-control/installer/README.md#ssh-host-key-changes).

The installer verifies payload, actual firmware and state; creates the backup; stops **only CC2 Control**; installs the ARM application/UI; restarts the service. Allow up to **240 seconds** for health, running-binary checksum, MQTT registration and printer snapshot.

Success prints **INSTALLATION PASSED**, the backup directory and exact restore command. If it fails, inspect rollback status and keep the backup path. Do not start a print until the previous or updated service works.

## 3. Verify

1. Open http://PRINTER-IP:8081 and reload; use Ctrl+F5 if the old interface persists.
2. Check **1.2.5**, **Printer connected**, temperatures and Canvas slots. The saved LAN code should not be requested again.
3. Select **Magyar** under **Settings → Appearance**. Eight languages are available.
4. Inspect plates/nozzles in **Bed Levelling**. Calibration accepts bed temperature **40–110 °C**, soak **0–60 minutes** (default **10**) and nozzle. Keep the bed clear before confirming.
5. Each plate keeps measurements by temperature and nozzle. The print dialog uses the nearest bed-temperature measurement or your chosen measurement, with the selected nozzle Z correction.
6. Restart once while Idle, then verify automatic startup, connection and Canvas again.

A factory reset and LAN-code replacement are not required. The printer display retains vendor base 02.01.01.07; the application version is 1.2.5.

## 4. Restore the previous application

While connected, Idle and cold with both heater targets zero, use the exact backup printed by your installation:

~~~sh
ssh root@PRINTER-IP
sh /opt/usr/cc2-control-backup-YOUR-TIMESTAMP-PID/restore.sh
~~~

Replace IP and backup placeholders. Current credentials, materials, preferences, plates/nozzles and spools are retained. For a firmware-integrated predecessor the original launcher can return to the binary under /opt/inst; the /opt/usr backup may contain only persistent data.

This restores the **application**, not an older ELEGOO firmware. Firmware downgrade restrictions remain unchanged. Do not reuse another printer backup.
