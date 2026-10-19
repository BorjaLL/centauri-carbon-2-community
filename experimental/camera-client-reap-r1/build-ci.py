import hashlib,json,os,re,shutil,subprocess,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
root=Path(__file__).resolve().parent
os.chdir(root)
out=root/'output';out.mkdir(exist_ok=True)
def run(args,**kw):subprocess.run(list(map(str,args)),check=True,timeout=180,**kw)
flags=['-O2','-std=c11','-Wall','-Wextra','-Wpedantic','-Werror','-pthread']
run(['gcc',*flags,'reap_core.c','test_reap.c','-o',out/'test-reap'])
run([out/'test-reap'])
run(['gcc',*flags,'-fsanitize=address,undefined','-fno-omit-frame-pointer','reap_core.c','test_reap.c','-o',out/'test-reap-asan'])
run([out/'test-reap-asan'],env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1:abort_on_error=1'))
run(['gcc',*flags,'-c','reap_core.c','-o',out/'reap-core.o'])
run(['g++','-O2','-std=c++17','-Wall','-Wextra','-Werror','-pthread','test_abi.cpp',out/'reap-core.o','-o',out/'test-abi'])
run([out/'test-abi'])
run(['gcc',*flags,'-fPIC','-shared','sha256_file.c','-o',out/'hash-test.so'])
run([sys.executable,'test_hash.py',out/'hash-test.so'])
zig=Path(os.environ['CC2_ZIG'])
arm=[zig,'cc','-target','arm-linux-gnueabihf.2.23','-mcpu=cortex_a7','-marm',*flags]
library=out/'libcc2-camera-reap-r1.so'
run([*arm,'-fPIC','-shared','camera_reap.c','reap_core.c','sha256_file.c','-ldl','-o',library])
run([*arm,'reap_core.c','test_reap.c','-o',out/'test-reap-arm'])
run(['qemu-arm','-L','/usr/arm-linux-gnueabihf',out/'test-reap-arm'])
run(['arm-linux-gnueabihf-gcc',*flags,'-c','reap_core.c','-o',out/'reap-core-arm.o'])
run(['arm-linux-gnueabihf-g++','-O2','-std=c++17','-Wall','-Wextra','-Werror','-pthread','test_abi.cpp',out/'reap-core-arm.o','-o',out/'test-abi-arm'])
run(['qemu-arm','-L','/usr/arm-linux-gnueabihf',out/'test-abi-arm'])
run([*arm,'sha256_file.c','sha256_cli.c','-o',out/'sha256-arm'])
expected=hashlib.sha256(library.read_bytes()).hexdigest()
actual=subprocess.check_output(['qemu-arm','-L','/usr/arm-linux-gnueabihf',str(out/'sha256-arm'),str(library)],text=True).strip()
assert actual==expected
with library.open('rb') as f:
    elf=ELFFile(f)
    assert elf.elfclass==32 and elf.header['e_machine']=='EM_ARM' and elf.header['e_type']=='ET_DYN'
    assert elf.header['e_flags']&0x400
    versions=subprocess.check_output(['arm-linux-gnueabihf-readelf','-V',str(library)],text=True)
    required=[tuple(map(int,x.split('.'))) for x in re.findall(r'GLIBC_(\d+\.\d+(?:\.\d+)?)',versions)]
    assert required and max(required)<=(2,23),versions
    (out/'required-symbol-versions.txt').write_text(versions)
for name in ('trial.sh','README.it.md','README.en.md'):
    shutil.copyfile(root/name,out/name)
run(['sh','-n','trial.sh'])
result={'library_sha256':expected,'qualified_camera_sha256':'ec5633b17de329d5b18c3f8d8de6ebf389391a04193536d976ba167022d07b99',
        'glibc_max_required':'.'.join(map(str,max(required))),'host_threads':1200,'arm_emulated_threads':1200,
        'real_libstdcxx_abi_host':True,'real_libstdcxx_abi_arm_qemu':True,
        'asan_ubsan':True,'sha256_host_and_arm':True,
        'printer_qualified':False,'camera_streaming_qualified':False,
        'activation':'temporary only; no persistent startup change','lan_enable_once':'explicitly enabled by trial launcher for a fresh process'}
(out/'validation.json').write_text(json.dumps(result,indent=2)+'\n')
package=out/'package';package.mkdir(exist_ok=True)
for name in ('libcc2-camera-reap-r1.so','trial.sh','README.it.md','README.en.md','validation.json','required-symbol-versions.txt'):
    shutil.copyfile(out/name,package/name)
(package/'SHA256SUMS').write_text(''.join(hashlib.sha256(p.read_bytes()).hexdigest()+'  '+p.name+'\n' for p in sorted(package.iterdir()) if p.name!='SHA256SUMS'))
print(json.dumps(result,indent=2))
