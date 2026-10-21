"""Boot-only camera component guards and rollback on two qualified profiles."""
import os
from pathlib import Path
import subprocess
import sys

test=Path(__file__).resolve().parents[1]/'installer/camera/test_persistent.py'
for native in ('0','1'):
    subprocess.run([sys.executable,str(test)],check=True,
                   env=dict(os.environ,CC2_TEST_NATIVE_PROFILE=native))
