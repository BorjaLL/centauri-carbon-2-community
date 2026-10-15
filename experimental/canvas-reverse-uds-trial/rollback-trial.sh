#!/bin/sh
set -eu
BASE=/opt/usr/cc2-reactor-runtime-v2
BACKUP=$BASE/canvas-reverse-trial-backup
[ -f "$BACKUP/launcher.sh" ] && [ -f "$BACKUP/SHA256SUMS" ] || { echo 'No complete rollback backup' >&2; exit 1; }
cmp -s "$BACKUP/SHA256SUMS" /opt/inst/cc2-reactor/SHA256SUMS || { echo 'Firmware component changed; old backup will not be restored over it' >&2; exit 1; }
[ "$(sha256sum "$BASE/elegoo_printer" | awk '{print $1}')" = 19712b5943b10bb8cb6aab9a4ed498ee962b28538b23c91b7ed003eb9e590745 ] || { echo 'Executable changed; refuse old rollback' >&2; exit 1; }
OLD=$(awk '$2=="launcher.sh" {print $1}' "$BACKUP/SHA256SUMS")
[ "$(sha256sum "$BACKUP/launcher.sh" | awk '{print $1}')" = "$OLD" ] || { echo 'Invalid launcher backup' >&2; exit 1; }
cp "$BACKUP/launcher.sh" "$BASE/launcher.sh.restore"
chmod 700 "$BASE/launcher.sh.restore"
cp "$BACKUP/SHA256SUMS" "$BASE/SHA256SUMS.restore"
mv "$BASE/launcher.sh.restore" "$BASE/launcher.sh"
mv "$BASE/SHA256SUMS.restore" "$BASE/SHA256SUMS"
(cd "$BASE" && sha256sum -c SHA256SUMS)
echo 'Original runtime launcher and checksum list restored. Reboot while idle.'
echo 'The current process keeps its in-memory trial until it exits; no motor command was sent.'
