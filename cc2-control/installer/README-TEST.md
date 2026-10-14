# CC2 Control 1.2.7 — combined test, V4.4 only

This updates CC2 Control only, leaving printer firmware, elegoo_printer and the
callback library in place. It supports only Community Firmware V4.4 based on
ELEGOO 02.01.01.07. The installer checks the actual installed and running binaries.

Includes PR117, 118, 119 and 120: installation with backup/rollback, Hungarian,
advanced bed mesh, available storage, 15-item list pages, optional persistent
live camera, and Canvas and OrcaSlicer metadata corrections. Previous fixes are
retained. PR103/114, continuous Canvas UDS and the unfinished Settings updater
are excluded. One acknowledged UDS command prepares each external-feed start;
Canvas telemetry continues to use MQTT.

## Install

1. Extract the entire ZIP on your computer. Do not send the ZIP to the printer.
2. Finish prints and calibrations; close live video. The printer must be
   configured, connected and idle, with both heaters at or below 50 °C and
   both heating targets zero.
3. Windows: run `Install-Windows.cmd` and follow its prompts. Linux/macOS:
   run `sh ./install-cc2-control.sh PRINTER-IP`.
4. SSH username is `root`; use your password, or the default `MTY4ODE2`.
   This is separate from the LAN code. Do not delete configuration: LAN code,
   preferences, materials, plates, nozzles and spools are retained.
5. Wait for the full installer result. It creates a backup and checks the
   version, running executable and MQTT connection. Keep the printed backup
   directory and exact restore command.
6. Reload the browser and confirm version 1.2.7. Restart once while idle and
   recheck automatic startup, connection and Canvas.

Do not bypass a compatibility or printer-state refusal. If installation fails,
keep the first error and rollback outcome. Do not factory-reset for this trial.

## Checks before merge

- Hungarian, advanced mesh and the previously working commands/quick actions.
- Internal/USB capacity, the second Files/History page, filtering and selection.
- Settings → Appearance: enable persistent live video, start it and change
  pages. Use the normal Dashboard/Job card; no floating window is created.
  Minimize the browser: video must continue. Pause, disabling the preference
  or closing the page stops it; reload still requires a new manual start.
  pages. Disabling the option should restore the previous camera behaviour.
- The reported OrcaSlicer file should show **55 °C bed**, **220 °C nozzle**
  and **21.2 g estimated filament**. This is separate from measured spool usage.
- PR119: an external-feed start after a previous mapped Canvas job, following
  the printer's normal external-feed setup; then a calibrated multicolour start
  with every tool mapped. A failed map-clear acknowledgement must refuse start.

The author's external-feed test passed on V4.2-R5; V4.4 and calibrated multicolour
behaviour require this trial. PRs stay open until the user's printer test is
confirmed. This package does not replace the published official release.

## Credits

@beanbo for advanced mesh and PR119; @pezso92 for Hungarian; thanks to the beta
tester who supplied the G-code for PR120 and all testers credited by the project.
The integration retains the original PR119 commits.
