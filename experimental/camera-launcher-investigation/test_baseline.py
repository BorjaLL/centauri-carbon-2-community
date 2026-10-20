"""Refusal/cancellation tests with shell functions replacing all process operations."""
import os
from pathlib import Path
import shlex
import subprocess
import tempfile

root = Path(__file__).resolve().parent
source = (root / 'unpatched-baseline.sh').read_text()
subprocess.run(['sh', '-n', str(root / 'unpatched-baseline.sh')], check=True)
assert 'LD_PRELOAD= CC2_CAMERA_REAP_TRIAL=0 CC2_CAMERA_REAP_ENABLE_LAN=0 /opt/bin/ai_camera' in source
assert source.index('    check\n    OLD_PID=') < source.index('    kill -TERM "$PID"')
assert 'killall ai_camera' not in source
assert 'reboot\n' not in source
assert source.count('kill -TERM') == 1
prefix, command = source.split('case "${1:-check}" in', 1)
with tempfile.TemporaryDirectory(prefix='cc2-baseline-fixture-') as td:
    work = Path(td)
    marker = work / 'signal.txt'
    mocks = '''
camera_pid() { PID=1234; }
qualified() { [ "$MODE" != wrong-camera ]; }
idle() { [ "$MODE" != busy ]; }
dependencies() { [ "$MODE" != missing-library ]; }
readlink() { printf '%s\n' "$WORK"; }
status() { echo fixture-status; }
kill() { printf '%s\n' "$*" >> "$WORK/signal.txt"; return 47; }
'''
    fixture = work / 'fixture.sh'
    fixture.write_text(prefix + '\nWORK=' + shlex.quote(td) + '\n' + mocks + '\ncase "${1:-check}" in' + command)
    for mode, action, answer, code, signalled in [
        ('good', 'check', '', 0, False),
        ('wrong-camera', 'start', 'RESTART CAMERA\n', 1, False),
        ('busy', 'start', 'RESTART CAMERA\n', 1, False),
        ('missing-library', 'start', 'RESTART CAMERA\n', 1, False),
        ('good', 'start', 'cancel\n', 0, False),
        ('good', 'start', '', 0, False),
        ('good', 'start', 'RESTART CAMERA\n', 47, True),
    ]:
        marker.unlink(missing_ok=True)
        result = subprocess.run(['sh', str(fixture), action], input=answer, env=dict(os.environ, MODE=mode), capture_output=True, text=True, timeout=10)
        assert result.returncode == code, (mode, action, result)
        assert marker.exists() == signalled, (mode, answer)
        if signalled:
            assert marker.read_text() == '-TERM 1234\n'
    # Exercise the actual freshness/idle parser without HTTP or printer hardware.
    idle_fixture = work / 'idle.sh'
    idle_fixture.write_text(prefix + '\nwget() { printf \'%s\' "$STATE_FIXTURE"; }\nidle\n')
    for state, accepted in [
        ('{"connected":true,"last_message_age":2,"machine":{"status":1,"status_name":"Idle"}}', True),
        ('{"connected":true,"last_message_age":16,"machine":{"status":1,}}', False),
        ('{"connected":false,"last_message_age":0,"machine":{"status":1,}}', False),
        ('{"connected":true,"last_message_age":0,"machine":{"status":3,}}', False),
        ('{"connected":true,"machine":{"status":1,}}', False),
    ]:
        result = subprocess.run(['sh', str(idle_fixture)], env=dict(os.environ, STATE_FIXTURE=state), capture_output=True, text=True, timeout=10)
        assert (result.returncode == 0) == accepted, (state, result)
print('PASS: original-camera restart preconditions, refusal/cancellation/EOF, single exact-PID signal boundary and actual freshness parser. No real camera or process signalled.')
