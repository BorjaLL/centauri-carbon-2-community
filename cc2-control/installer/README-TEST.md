# CC2 Control 1.2.8 — updater trial, V4.4 only

This retains all validated 1.2.7 changes and updates CC2 Control only.
Printer firmware and the callback library are unchanged.

1. Extract the entire ZIP on your computer.
2. Wait for an idle printer, nozzle and bed ≤50 °C, both targets zero; pause live view.
3. Run `Install-Windows.cmd`; on Linux/macOS use
   `sh ./install-cc2-control.sh PRINTER-IP`.
4. Use root and your SSH password.
   Do not delete configuration: profiles, inventory and LAN settings are retained.
5. Wait for the installer result and keep the backup/restore instructions.
6. Reload with Ctrl+F5, check version 1.2.8 and reboot once while idle.
   Recheck connection, Canvas and persistent camera.

Open Settings → Updates. Stable must be the initial channel. The online index
is published only when packages are ready; an absent index is not a printer
failure. To test a future signed version, upload its `.cc2update` file.
Verification alone must not install anything. Installation needs a separate
confirmation. The LAN code is distinct from the SSH password. ZIP files are
not panel-installable updates.

Full English and Italian guides are included in the source archive:
`docs/APPLICATION_UPDATES.md` and `docs/APPLICATION_UPDATES.it.md`.
Before merging, qualify HTTPS downloads, altered/incompatible package rejection,
signed installation, reboot and configuration retention on the printer.
Do not perform a factory reset for this trial.
