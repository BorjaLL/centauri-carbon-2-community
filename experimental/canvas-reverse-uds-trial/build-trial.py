"""Build local-only experimental runtime overlay. No vendor binary is bundled."""
from pathlib import Path
import hashlib
import json
import os
import re
import shutil
import struct
import subprocess
import tempfile

HERE=Path(__file__).resolve().parent
COMPILER=Path('/opt/cc2-cross/toolchain-out/bin/arm-cortex_a15-linux-gnueabihf-gcc')
FLAGS=['-O2','-std=c11','-Wall','-Wextra','-Wpedantic','-Werror']


def elf(path,kind):
    data=path.read_bytes()
    assert data[:6]==b'\x7fELF\x01\x01'
    assert struct.unpack_from('<HH',data,16)==(kind,40)
    assert struct.unpack_from('<I',data,36)[0]&0x400


def main():
    if not COMPILER.is_file():
        raise RuntimeError('Existing builder ARM compiler not found: '+str(COMPILER))
    subprocess.run(['python3',str(HERE/'test_install.py')],check=True,timeout=30)
    for name in ('install-trial.sh','rollback-trial.sh','check-trial.sh','launcher.original.sh','launcher.trial.sh'):
        subprocess.run(['sh','-n',str(HERE/name)],check=True)
    with tempfile.TemporaryDirectory(prefix='cc2-reverse-check-') as temp:
        for name in ('test_motion','test_rpc'):
            binary=Path(temp)/name
            subprocess.run(['gcc',*FLAGS,'-pthread',str(HERE/(name+'.c')),'-o',str(binary),'-lm'],check=True)
            subprocess.run([str(binary)],check=True,timeout=10)
        subprocess.run(['gcc',*FLAGS,'-fsyntax-only',str(HERE/'reverse_trial.c')],check=True)
    output=HERE/'output'
    output.mkdir(exist_ok=True)
    library=output/'libcc2-canvas-reverse-trial.so'
    motion=output/'cc2-canvas-motion-probe'
    subprocess.run([str(COMPILER),*FLAGS,'-mcpu=cortex-a7','-mfpu=neon-vfpv4','-mfloat-abi=hard','-fPIC','-shared',str(HERE/'reverse_trial.c'),'-o',str(library)],check=True)
    subprocess.run([str(COMPILER),*FLAGS,'-mcpu=cortex-a7','-mfpu=neon-vfpv4','-mfloat-abi=hard','-fno-pie','-no-pie',str(HERE/'motion_probe.c'),'-o',str(motion),'-lm'],check=True)
    elf(library,3)
    elf(motion,2)
    for path in (library,motion):
        listing=subprocess.check_output([str(COMPILER.with_name(COMPILER.name.removesuffix('gcc')+'readelf')),'-V',str(path)],text=True)
        versions=[tuple(map(int,v.split('.'))) for v in re.findall(r'GLIBC_(\d+\.\d+(?:\.\d+)?)',listing)]
        if any(v>(2,23) for v in versions):
            raise RuntimeError('Requires a newer glibc than the printer: '+str(path))
        os.chmod(path,0o700)
    for name in ('install-trial.sh','rollback-trial.sh','check-trial.sh','launcher.original.sh','launcher.trial.sh'):
        shutil.copyfile(HERE/name,output/name)
        os.chmod(output/name,0o700)
    names=sorted(p.name for p in output.iterdir() if p.is_file() and p.name!='SHA256SUMS')
    (output/'SHA256SUMS').write_text(''.join(hashlib.sha256((output/n).read_bytes()).hexdigest()+'  '+n+'\n' for n in names),encoding='utf-8')
    print('BUILD PASS: '+str(output))
    print('No printer modified. Follow the first-boot checks before any pulse.')


if __name__=='__main__':
    main()
