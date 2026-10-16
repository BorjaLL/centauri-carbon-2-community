import hashlib,json,subprocess,zipfile
from pathlib import Path
root=Path(__file__).resolve().parents[2];app=root/'cc2-control';out=root/'output/combined-1214';out.mkdir(parents=True,exist_ok=True)
source=out/'CC2-Control-1.2.14-Sources.zip'
names=subprocess.check_output(['git','ls-files','-z','cc2-control'],cwd=root).decode().split('\0')
with zipfile.ZipFile(source,'w',zipfile.ZIP_DEFLATED,compresslevel=9) as z:
    for name in sorted(filter(None,names)):
        p=root/name
        if not p.is_file(): continue
        info=zipfile.ZipInfo(p.relative_to(app).as_posix(),(2026,1,1,0,0,0));info.compress_type=zipfile.ZIP_DEFLATED
        info.external_attr=(0o100755 if p.suffix in ('.sh','.init') else 0o100644)<<16;z.writestr(info,p.read_bytes())
subprocess.run(['python3','installer/package.py','--prepared','dist/cc2-control','--source',str(source),'--output',str(out/'CC2-Control-1.2.14-Multiplatform-Update.zip')],cwd=app,check=True)
record={'version':'1.2.14','firmware':'Community V4.4 / V4.4.1 only','candidate_commit':subprocess.check_output(['git','rev-parse','HEAD'],cwd=root,text=True).strip(),'coordinate_fix_sha256':hashlib.sha256((root/'experimental/combined-1212/object-coordinates-fix.patch').read_bytes()).hexdigest(),'plate_fix_sha256':hashlib.sha256((root/'experimental/combined-1212/plate-workflow-fix.patch').read_bytes()).hexdigest(),'patch_sha256':hashlib.sha256((root/'experimental/combined-1212/changes.patch').read_bytes()).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'status':'host/browser/ARM tests passed; real printer validation pending','credits':{'PR128':'Surfoo'},'experimental_canvas_ejection_included':False,'webcam_service_restart_included':False}
(out/'qualification.json').write_text(json.dumps(record,indent=2)+'\n')
