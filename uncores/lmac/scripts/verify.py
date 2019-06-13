#!/usr/bin/python

import argparse
import os
import os.path
import stat
import subprocess


def Verify(top):
    if not os.path.exists(top):
        print('dir {0} does not exist'.format(top))
        return

    cwd = os.getcwd()

    for root, dirs, _ in os.walk(top):
        for name in dirs:
            dir_path = os.path.join(root, name)

            # check if contains 'do.tcl' & 'run.sh' 
            jg_script = os.path.join(dir_path, 'do.tcl')
            if not os.path.isfile(jg_script): 
                continue

            run_script = os.path.join(dir_path, 'run.sh')
            if not os.path.isfile(run_script):
                continue

            print('Verify instruction {}'.format(name))

            # modify default run script
            new_jg_script = os.path.join(dir_path, 'verify.tcl')
            subprocess.call(['cp', jg_script, new_jg_script])
            with open(new_jg_script, 'a') as fw:
                fw.write('prove -all\n')
                fw.write('exit\n')

            new_run_script = os.path.join(dir_path, 'run_verify.sh')
            with open(new_run_script, 'w') as fw:
                fw.write('jg -no_gui -fpv verify.tcl &> result.log\n')
            st = os.stat(new_run_script)
            os.chmod(new_run_script, st.st_mode | stat.S_IEXEC)

            # verify
            os.chdir(dir_path)
            subprocess.call('run_verify.sh', shell=True)
            os.chdir(cwd)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Run verification')
    parser.add_argument('--all', action='store_true', help='run all')
    parser.add_argument('--reg', action='store_true', help='run reg')
    args = parser.parse_args()

    if (args.all or args.reg):
        Verify('verification/core2/reg')
