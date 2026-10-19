"""Resolve only generated outputs and known additive acknowledgements.
Any source conflict outside this allowlist requires manual review.
"""
import subprocess
from pathlib import Path
conflicts=subprocess.check_output(['git','diff','--name-only','--diff-filter=U'],text=True).splitlines()
allowed={
 'builder/current/components/cc2-control/source/source.zip',
 'builder/current/prepare.py','builder/current/core/firmware_builder.py',
 'builder/current/launch_helpers/run_build.py','docs/CONTRIBUTORS.md',
 'cc2-control/web/index.html',
 'cc2-control/firmware-integration/overlay/opt/usr/cc2-control/web/index.html',
}
assert set(conflicts)<=allowed,conflicts
for name in conflicts:
 subprocess.run(['git','checkout','--ours','--',name],check=True)
 if name=='docs/CONTRIBUTORS.md':
  p=Path(name)
  p.write_text(p.read_text()+'\n## Panda Breath integration\n\n**[Noahwhite2014](https://github.com/Noahwhite2014)** contributed the optional Panda Breath / Dragon Breath interface ([#166](https://github.com/Neme77/centauri-carbon-2-community/pull/166), integrated in [#167](https://github.com/Neme77/centauri-carbon-2-community/pull/167)). Original contribution history and feature attribution are preserved. Thanks to Barry for real-device feedback.\n')
 subprocess.run(['git','add','--',name],check=True)

