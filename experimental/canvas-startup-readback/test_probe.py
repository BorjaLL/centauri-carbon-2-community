import concurrent.futures, json, pathlib, socket, subprocess, tempfile

here=pathlib.Path(__file__).resolve().parent
expected={'id':7471,'method':'objects/query','params':{'objects':{
    'canvas_dev':None,'filament_switch_sensor filament_sensor':None,
    'extruder':None,'print_stats':['state']}}}
with tempfile.TemporaryDirectory() as temporary:
    root=pathlib.Path(temporary);binary=root/'probe'
    subprocess.run(['gcc','-std=c11','-O2','-Wall','-Wextra','-Wpedantic','-Werror',str(here/'probe.c'),'-o',str(binary),'-lm'],check=True)
    assert subprocess.run([str(binary),'a','b'],capture_output=True).returncode==2
    for case in ['fragmented','error','incomplete','oversize']:
        path=str(root/case)
        with socket.socket(socket.AF_UNIX,socket.SOCK_STREAM) as listener:
            listener.bind(path);listener.listen(1);listener.settimeout(5)
            def serve():
                conn,_=listener.accept()
                with conn:
                    conn.settimeout(5);data=b''
                    while b'\x03' not in data:data+=conn.recv(4096)
                    assert data.endswith(b'\x03') and json.loads(data[:-1])==expected
                    if case=='incomplete':conn.sendall(b'{"id":7471');return
                    if case=='oversize':
                        try:conn.sendall(b'x'*66000+b'\x03')
                        except (BrokenPipeError,ConnectionResetError):pass
                        return
                    message={'id':7471,'error':{'message':'refused'}} if case=='error' else {
                        'id':7471,'result':{'status':{'canvas_dev':{'active_cid':-1},
                        'filament_switch_sensor filament_sensor':{'enabled':True,'filament_detected':True}}}}
                    wire=b'{"id":0,"method":"unrelated"}\x03'+json.dumps(message).encode()+b'\x03'
                    conn.sendall(wire[:7]);conn.sendall(wire[7:])
                    assert conn.recv(4096)==b'', 'client must close without a second request'
            with concurrent.futures.ThreadPoolExecutor(1) as pool:
                future=pool.submit(serve)
                result=subprocess.run([str(binary),path],capture_output=True,text=True,timeout=10)
                future.result(timeout=6)
            assert result.returncode=={'fragmented':0,'error':6,'incomplete':4,'oversize':5}[case],(case,result)
            if case in ['fragmented','error']:assert json.loads(result.stdout)['id']==7471
    print('PASS: fixed read-only query, fragmented/unrelated/error/incomplete/oversize replies; no follow-up request')
