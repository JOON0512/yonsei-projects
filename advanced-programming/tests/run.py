"""CPU tests; g++, MPI and Python 3 required. CUDA needs separate hardware."""
import os,subprocess,tempfile
from pathlib import Path
root=Path(__file__).resolve().parents[1]
def run(args,cwd=root,**kwargs):
    return subprocess.run(args,cwd=cwd,check=True,text=True,timeout=60,**kwargs)
with tempfile.TemporaryDirectory() as temporary:
    temp=Path(temporary)
    for folder,exe in [('class','class'),('template','vector')]:
        run(['g++','-std=c++17','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*[str(p) for p in (root/folder).glob('*.cc')],'-o',str(temp/exe)])
        result=run([str(temp/exe)],cwd=root/folder,capture_output=True)
        if folder=='class':
            assert 'X (size=6' in result.stdout and 'Y (size=13' in result.stdout
        else:assert '#9. words' in result.stdout and 'size()=0' in result.stdout
        print('PASS:',folder,'sample and ASan/UBSan',flush=True)
    run(['g++','-std=c++17','-fsanitize=address,undefined','-iquote'+str(root/'template'),str(root/'tests/check_vector.cc'),'-o',str(temp/'check-vector')])
    run([str(temp/'check-vector')])
    print('PASS: vector empty/nonempty copy and independent lifetime',flush=True)
    run(['g++','-std=c++17','-pthread','-fsanitize=address,undefined','-iquote'+str(root/'thread'),str(root/'tests/check_sort.cc'),'-o',str(temp/'check-sort')])
    run([str(temp/'check-sort')])
    run(['make'],cwd=root/'thread')
    invalid=subprocess.run(['./thread','0'],cwd=root/'thread',text=True,capture_output=True,timeout=3)
    assert invalid.returncode!=0 and 'between 1 and 1024' in invalid.stderr
    print('PASS: zero thread count rejected',flush=True)
    run(['mpic++','-std=c++17','-iquote'+str(root/'mpi'),str(root/'tests/check_mpi.cc'),'-o',str(temp/'check-mpi')])
    env=os.environ.copy()
    if os.geteuid()==0:env.update(OMPI_ALLOW_RUN_AS_ROOT='1',OMPI_ALLOW_RUN_AS_ROOT_CONFIRM='1')
    for ranks in [1,2,3,6]:run(['mpiexec','--oversubscribe','-np',str(ranks),str(temp/'check-mpi')],env=env)
print('ADVANCED_CPU_TEST_SUCCESS')
