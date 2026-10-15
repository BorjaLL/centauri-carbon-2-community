#!/bin/sh
set -eu
umask 077
HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
BASE=/opt/usr/cc2-reactor-runtime-v2
SOURCE=/opt/inst/cc2-reactor
BACKUP=$BASE/canvas-reverse-trial-backup
EXPECTED=19712b5943b10bb8cb6aab9a4ed498ee962b28538b23c91b7ed003eb9e590745
fail() { echo "STOP: $*" >&2; exit 1; }
[ "$(id -u)" = 0 ] || fail 'root required'
[ ! -e "$BACKUP" ] || fail 'Backup already exists; do not overwrite it. Continue checks or use rollback'
[ "$(sha256sum "$BASE/elegoo_printer" | awk '{print $1}')" = "$EXPECTED" ] || fail 'Different printer executable'
cmp -s "$BASE/launcher.sh" "$HERE/launcher.original.sh" || fail 'Different runtime launcher'
cmp -s "$BASE/SHA256SUMS" "$SOURCE/SHA256SUMS" || fail 'Runtime differs from installed firmware component'
(cd "$BASE" && sha256sum -c SHA256SUMS) || fail 'Existing runtime verification failed'
awk '
BEGIN {
    want["libcc2-canvas-reverse-trial.so"]=1; want["cc2-canvas-motion-probe"]=1;
    want["install-trial.sh"]=1; want["rollback-trial.sh"]=1; want["check-trial.sh"]=1;
    want["launcher.original.sh"]=1; want["launcher.trial.sh"]=1;
}
NF!=2 || length($1)!=64 || $1!~/^[0-9a-f]+$/ || !($2 in want) || seen[$2]++ {bad=1}
END {for(name in want) if(!seen[name]) bad=1; exit bad}
' "$HERE/SHA256SUMS" || fail 'Malformed, incomplete or unexpected trial manifest'
(cd "$HERE" && sha256sum -c SHA256SUMS) || fail 'Trial package verification failed'
mkdir "$BACKUP"
cp "$BASE/launcher.sh" "$BACKUP/launcher.sh"
cp "$BASE/SHA256SUMS" "$BACKUP/SHA256SUMS"
cp "$HERE/libcc2-canvas-reverse-trial.so" "$BASE/libcc2-canvas-reverse-trial.so.new"
chmod 700 "$BASE/libcc2-canvas-reverse-trial.so.new"
mv "$BASE/libcc2-canvas-reverse-trial.so.new" "$BASE/libcc2-canvas-reverse-trial.so"
cp "$HERE/launcher.trial.sh" "$BASE/launcher.sh.new"
chmod 700 "$BASE/launcher.sh.new"
NEW_LAUNCHER=$(sha256sum "$BASE/launcher.sh.new" | awk '{print $1}')
awk -v sum="$NEW_LAUNCHER" '$2=="launcher.sh" {$1=sum} {print $1 "  " $2}' "$BACKUP/SHA256SUMS" > "$BASE/SHA256SUMS.new"
(cd "$BASE" && sha256sum libcc2-canvas-reverse-trial.so) >> "$BASE/SHA256SUMS.new"
mv "$BASE/launcher.sh.new" "$BASE/launcher.sh"
mv "$BASE/SHA256SUMS.new" "$BASE/SHA256SUMS"
cp "$HERE/rollback-trial.sh" "$BACKUP/rollback-trial.sh"
(cd "$BASE" && sha256sum -c SHA256SUMS) || fail 'Staged runtime verification failed; restore with rollback'
echo 'STAGED: no service restarted and no motor command sent.'
echo 'Reboot manually only with the printer idle. Then run check-trial.sh.'
echo "Rollback: sh $BACKUP/rollback-trial.sh"
