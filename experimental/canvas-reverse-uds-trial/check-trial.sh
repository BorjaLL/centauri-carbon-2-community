#!/bin/sh
set -eu
BASE=/opt/usr/cc2-reactor-runtime-v2
(cd "$BASE" && sha256sum -c SHA256SUMS)
echo 'Printer PID:'
pidof elegoo_printer
echo 'Callback activation:'
cat "$BASE/activation.marker"
echo 'Canvas reverse activation:'
cat "$BASE/canvas-reverse.marker"
echo 'Service health:'
wget -qO- http://127.0.0.1:8081/api/health
echo
echo 'Run the included motion tool in check mode. Physical direction/stop remain unverified.'
