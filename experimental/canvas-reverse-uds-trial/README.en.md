# Canvas reverse trial R1: one short slot-2 pulse

Experimental local-only trial for V4.4 / V4.4.1 with executable SHA256 `19712b5943b10bb8cb6aab9a4ed498ee962b28538b23c91b7ed003eb9e590745`.

This is not full automatic ejection. It targets **10 mm reverse on slot 2 / protocol channel 1**, SPEED=5, ACCEL=100, TIMEOUT=3. It does not command the measured 840 mm, repeat automatically, or expose adjustable motion parameters.

The module patches only the negative-distance comparison in memory during qualified startup. Executable bytes on disk and the callback library stay unchanged. The runtime launcher adds the trial library while retaining executable/component checksum gates, the original executable path and callback activation. The original launcher/checksum list are backed up. No vendor binaries, credentials or signing keys are included.

## Build and stage

Extract on the Windows builder PC and run **Build-Trial.cmd**. Ubuntu WSL and the existing ARM compiler at `/opt/cc2-cross/toolchain-out/bin/arm-cortex_a15-linux-gnueabihf-gcc` are required. Host guard/socket tests and script syntax checks run before ARM compilation and glibc compatibility checks. Stop on any error. The source ZIP does not include precompiled ARM binaries.

Finish printing. Unload slot 2 through the normal display operation, leaving filament present in the Canvas but absent from the toolhead, with no active channel. Disable automatic insertion, set heater targets to zero, and wait until nozzle/bed temperatures are at most 50 C. Do not start display/slicer operations during the trial.

In PowerShell from the extracted package directory, substitute your printer IP:

```powershell
scp -O -r .\output root@192.168.1.103:/tmp/cc2-canvas-reverse-r1
```

When the destination does not exist, this copies `output` as `/tmp/cc2-canvas-reverse-r1`. Verify that `install-trial.sh` is directly inside it.

Over SSH:

```sh
sh /tmp/cc2-canvas-reverse-r1/install-trial.sh
```

Staging requires the pristine runtime launcher/checksum list to match the installed firmware component. It does not restart services or move motors. Backups remain under `/opt/usr/cc2-reactor-runtime-v2/canvas-reverse-trial-backup`.

## First boot: verify before motion

Reboot manually while idle:

```sh
reboot
```

Reconnect. `/tmp` may have been cleared; copy `output` again using the same PowerShell command. Do not rerun installation.

```sh
sh /tmp/cc2-canvas-reverse-r1/check-trial.sh
chmod 700 /tmp/cc2-canvas-reverse-r1/cc2-canvas-motion-probe
/tmp/cc2-canvas-reverse-r1/cc2-canvas-motion-probe check
```

Share these results before the first pulse. Both activation markers must match the live printer PID and normal CC2 Control connectivity must work. `check` sends no motion commands. Missing or unsuitable state blocks the pulse.

## One pulse after first-boot checks

```sh
/tmp/cc2-canvas-reverse-r1/cc2-canvas-motion-probe pulse-slot2
```

It requests the exact text **SLOT2** in an interactive SSH terminal and rechecks state before sending. Any other input cancels. It sends one reverse command followed by an explicit stop, including after command failure or Ctrl+C, and never retries motion.

Mark the filament at the Canvas inlet beforehand. Do not pull or restrain it during the pulse. Report actual direction, marked travel, whether the feeder physically stopped, and the program output. Actual travel can differ slightly due to control cadence and inertia. An ACK is not physical-stop feedback. Do not repeat after unexpected behavior; use the printer stop if movement does not stop.

## Restore

At idle, use the persistent backup even if `/tmp` has disappeared:

```sh
sh /opt/usr/cc2-reactor-runtime-v2/canvas-reverse-trial-backup/rollback-trial.sh
reboot
```

The running process retains its memory patch until exit. The original launcher is restored for the next boot. Extra trial files remain but the original launcher does not load them. Backups are not deleted automatically.

## Qualification

Initial ARM emulation passed 144 comparison cases, reproduced the original bug and preserved forward behavior. See `VALIDATION.json` for additional local checks when present. The user observed slot-2 UDS presence changing from 1 to 0 during manual removal and measured about 840 mm remaining after normal unload.

Host/ARM build checks run on the builder PC unless additional results are recorded. Simultaneous callback/module startup and physical direction, travel and stop remain untested on the printer. This does not qualify full ejection, the other slots, stall handling or the complete PR114.

Original ejection proposal and comparison-fix principle: **sauadhi**, PR103. Qualification of this new-base trial remains separate.
