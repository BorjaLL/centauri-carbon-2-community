"""Optional PC-only reproduction against a privately held qualified executable.

Requires unicorn and pyelftools on the analysis PC. Never installs on a printer.
"""
from pathlib import Path
import argparse
import hashlib
import json
import random
import struct
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM
from unicorn.arm_const import UC_ARM_REG_C1_C0_2, UC_ARM_REG_FPEXC, UC_ARM_REG_D7, UC_ARM_REG_FP, UC_ARM_REG_R3, UC_ARM_REG_PC

HERE=Path(__file__).resolve().parent
manifest=json.loads((HERE/'qualification.json').read_text())


def emulate(block,start,distance,delta):
    u=Uc(UC_ARCH_ARM,UC_MODE_ARM)
    u.mem_map(start&~0xfff,0x2000)
    u.mem_write(start,block)
    u.mem_map(0x10000,0x2000)
    u.reg_write(UC_ARM_REG_FP,0x11000)
    u.mem_write(0x11000-0x124,struct.pack('<i',distance))
    u.reg_write(UC_ARM_REG_D7,struct.unpack('<Q',struct.pack('<d',delta))[0])
    u.reg_write(UC_ARM_REG_C1_C0_2,0xf<<20)
    u.reg_write(UC_ARM_REG_FPEXC,0x40000000)
    # Four original instructions compute magnitude/convert signed distance;
    # six original/patched comparison instructions, then the original BEQ.
    u.emu_start(start,0,count=11)
    return u.reg_read(UC_ARM_REG_R3),u.reg_read(UC_ARM_REG_PC)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input',required=True,type=Path)
    args=parser.parse_args()
    raw=args.input.read_bytes()
    assert hashlib.sha256(raw).hexdigest()==manifest['executable_sha256'],'Executable fingerprint differs'
    site=manifest['site']
    with args.input.open('rb') as f:
        elf=ELFFile(f)
        assert elf['e_type']=='ET_EXEC' and elf['e_machine']=='EM_ARM'
        segment=next(s for s in elf.iter_segments() if s['p_type']=='PT_LOAD' and s['p_vaddr']<=site<s['p_vaddr']+s['p_filesz'])
        offset=segment['p_offset']+site-segment['p_vaddr']
    assert offset==manifest['offset']
    before=bytes.fromhex(manifest['before']);after=bytes.fromhex(manifest['after'])
    assert raw[offset:offset+24]==before
    old=raw[offset-16:offset+28]
    new=old[:16]+after+old[40:]
    start=site-16
    assert emulate(old,start,-840,0)[0]==1,'Original bug not reproduced'
    rng=random.Random(441)
    distances=[-1400,-1200,-900,-840,-600,-10,-1,0,1,10,840,1400]
    distances += [rng.randint(-1400,1400) for _ in range(88)]
    cases=0
    for distance in distances:
        target=abs(distance)
        for magnitude in (0,target/2,max(0,target-.25),target,target+.25,target+10):
            for sign in (-1,1):
                result=emulate(new,start,distance,magnitude*sign)
                expected=int(magnitude>target)
                assert result==(expected,start+44 if expected else 0x5f20f0),(distance,magnitude,result)
                if distance>=0:assert emulate(old,start,distance,magnitude*sign)==result
                cases+=1
    print(json.dumps({'input_sha256':manifest['executable_sha256'],'arm_cases_passed':cases,
                      'original_bug_reproduced':True,'forward_preserved':True,'original_branch_destinations_preserved':True,
                      'vendor_file_modified':False,'physical_printer_test':'pending'}))


if __name__=='__main__':
    main()
