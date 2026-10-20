"""Linux shell fixtures: dependency preflight, no real camera/process operations."""
import os
from pathlib import Path
import shlex
import subprocess
import tempfile

root = Path(__file__).resolve().parent
source = (root / 'trial.sh').read_text()
assert source.count('export LD_LIBRARY_PATH=/opt/lib') == 1
assert source.index('    camera_dependencies\n') < source.index('    kill -TERM "$PID"')
assert '< /dev/null > "$BASE/ai_camera-trial.log"' in source
assert 'nohup' not in source
subprocess.run(['sh', '-n', str(root / 'trial.sh')], check=True)
subprocess.run(['sh', '-n', str(root / 'camera-readonly.sh')], check=True)
prefix = source.split('case "${1:-check}" in', 1)[0]
assert prefix != source
with tempfile.TemporaryDirectory(prefix='cc2-loader-fixture-') as td:
    work = Path(td)
    loader = work / 'loader'
    loader.write_text('''#!/bin/sh
printf '%s|%s|%s' "$LD_LIBRARY_PATH" "${LD_PRELOAD-unset}" "$CC2_CAMERA_REAP_TRIAL" > "$TEST_CAMERA_ENV"
case "$TEST_CAMERA_MODE" in
missing) echo 'libhv.so: cannot open shared object file'; exit 127 ;;
partial) echo 'libhv.so => not found'; exit 0 ;;
good) echo 'libhv.so => /opt/lib/libhv.so'; exit 0 ;;
esac
exit 99
''')
    loader.chmod(0o755)
    env = dict(os.environ)
    for name in ('LD_LIBRARY_PATH', 'LD_PRELOAD', 'CC2_CAMERA_REAP_TRIAL'):
        env.pop(name, None)
    env['TEST_CAMERA_ENV'] = str(work / 'environment.txt')
    script = work / 'fixture.sh'
    script.write_text(prefix + '\nBASE=' + shlex.quote(td) + '\nLOADER=' + shlex.quote(str(loader)) +
                      '\ncamera_dependencies\nprintf preflight-passed > "$BASE/reached"\n')
    for mode, expected in [('missing', False), ('partial', False), ('good', True)]:
        reached = work / 'reached'
        reached.unlink(missing_ok=True)
        result = subprocess.run(['sh', str(script)], env=dict(env, TEST_CAMERA_MODE=mode), capture_output=True, text=True, timeout=10)
        assert (result.returncode == 0) == expected, (mode, result)
        assert reached.exists() == expected, mode
        assert (work / 'environment.txt').read_text() == '/opt/lib||0', mode
    loader.unlink()
    reached.unlink()
    result = subprocess.run(['sh', str(script)], env=env, capture_output=True, text=True, timeout=10)
    assert result.returncode != 0 and not reached.exists()
print('PASS: clean SSH environment, fixed /opt/lib, no active preload in loader check, dependency failures stop preflight; no real process restarted.')
