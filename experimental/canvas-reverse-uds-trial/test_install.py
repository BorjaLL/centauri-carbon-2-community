"""Exercise staging and rollback on disposable fake paths, never real printer paths."""
from pathlib import Path
import hashlib
import os
import subprocess
import tempfile

HERE=Path(__file__).resolve().parent
EXPECTED='19712b5943b10bb8cb6aab9a4ed498ee962b28538b23c91b7ed003eb9e590745'


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    if os.name!='posix':
        raise RuntimeError('Installer fixtures require Linux/WSL')
    with tempfile.TemporaryDirectory(prefix='cc2-install-fixture-') as temp:
        root=Path(temp)
        runtime=root/'runtime'
        firmware=root/'firmware'
        package=root/'package'
        commands=root/'commands'
        for directory in (runtime,firmware,package,commands):directory.mkdir()
        # Explicit fixture substitutes for local identity and the unavailable
        # vendor executable. All component/list hashes still use real sha256sum.
        (commands/'id').write_text('#!/bin/sh\necho 0\n')
        (commands/'sha256sum').write_text(
            '#!/bin/sh\nif [ "$#" = 1 ] && [ "$1" = "'+str(runtime/'elegoo_printer')+'" ]; then\n'
            '  if [ "${CC2_FIXTURE_BAD_HASH:-0}" = 1 ]; then echo "000000  $1"; else echo "'+EXPECTED+'  $1"; fi\n'
            'else exec /usr/bin/sha256sum "$@"; fi\n')
        for p in commands.iterdir():p.chmod(0o755)
        env={**os.environ,'PATH':str(commands)+os.pathsep+os.environ['PATH']}
        original=(HERE/'launcher.original.sh').read_bytes()
        native={'launcher.sh':original,'libcc2-reactor-v2.so':b'unchanged callback fixture',
                'cc2-reactor-bridge-test':b'unchanged bridge fixture','preflight.sh':b'unchanged preflight fixture'}
        sums=''.join(sha(native[name])+'  '+name+'\n' for name in sorted(native))
        for name,data in native.items():(runtime/name).write_bytes(data)
        (runtime/'elegoo_printer').write_bytes(b'vendor fixture: never distributed binary')
        (runtime/'SHA256SUMS').write_text(sums)
        (firmware/'SHA256SUMS').write_text(sums)
        for name in ('install-trial.sh','rollback-trial.sh'):
            source=(HERE/name).read_text()
            assert source.count('BASE=/opt/usr/cc2-reactor-runtime-v2')==1
            source=source.replace('BASE=/opt/usr/cc2-reactor-runtime-v2','BASE='+str(runtime))
            source=source.replace('/opt/inst/cc2-reactor',str(firmware))
            assert 'BASE=/opt/' not in source
            (package/name).write_text(source)
        for name in ('launcher.original.sh','launcher.trial.sh'):(package/name).write_bytes((HERE/name).read_bytes())
        (package/'libcc2-canvas-reverse-trial.so').write_bytes(b'trial module fixture')
        (package/'SHA256SUMS').write_text(''.join(sha(p.read_bytes())+'  '+p.name+'\n' for p in sorted(package.iterdir()) if p.name!='SHA256SUMS'))

        def run(name,success,extra=None):
            result=subprocess.run(['sh',str(package/name)],env={**env,**(extra or {})},capture_output=True,text=True,timeout=10)
            assert (result.returncode==0)==success,result.stdout+result.stderr

        run('install-trial.sh',False,{'CC2_FIXTURE_BAD_HASH':'1'})
        assert not (runtime/'canvas-reverse-trial-backup').exists()
        package_sums=(package/'SHA256SUMS').read_text()
        (package/'SHA256SUMS').write_text(package_sums+'invalid checksum entry\n')
        run('install-trial.sh',False)
        assert not (runtime/'canvas-reverse-trial-backup').exists()
        (package/'SHA256SUMS').write_text(package_sums)
        run('install-trial.sh',True)
        for name in ('libcc2-reactor-v2.so','cc2-reactor-bridge-test','preflight.sh'):assert (runtime/name).read_bytes()==native[name]
        assert (runtime/'elegoo_printer').read_bytes()==b'vendor fixture: never distributed binary'
        assert (runtime/'launcher.sh').read_bytes()==(HERE/'launcher.trial.sh').read_bytes()
        staged=(runtime/'SHA256SUMS').read_bytes()
        run('install-trial.sh',False)
        assert (runtime/'SHA256SUMS').read_bytes()==staged
        (firmware/'SHA256SUMS').write_text(sums+'firmware changed\n')
        run('rollback-trial.sh',False)
        assert (runtime/'SHA256SUMS').read_bytes()==staged
        (firmware/'SHA256SUMS').write_text(sums)
        run('rollback-trial.sh',True)
        for name,data in native.items():assert (runtime/name).read_bytes()==data
        assert (runtime/'SHA256SUMS').read_text()==sums
        print('PASS: wrong input refusal, staged checksums, unchanged callback/executable, repeat refusal, changed-firmware refusal and exact rollback')


if __name__=='__main__':
    main()
