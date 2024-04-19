#!/usr/bin/env python

# Script to run macros with more points at low energies (for the C. Joy plot at low energy)

import os, sys
import glob, subprocess, shutil

def run_loop():
    VFEM = read_env_var('VFEM')
    make_directory(os.path.join(VFEM,'ElecBackScat'))
    os.chdir(os.path.join(VFEM,'ElecBackScat'))

    REFERENCE = read_env_var('REFERENCE')
    make_directory(REFERENCE)            
    os.chdir (REFERENCE)

    G4MY  = read_env_var('G4MY')
    EMSRC  = read_env_var('EMSRC')
    G4WORKDIR = os.path.join(EMSRC,'ElecBackScat', 'macro')

    rundir = 'AlLoop'
    make_directory(rundir)
    os.chdir (rundir)

    macfiles = [f.replace('.mac','') for f in ['00deg_emstandatdGS.mac', '00deg_emstandatdOpt0.mac', '00deg_emstandatdOpt3.mac', '00deg_emstandatdSS.mac']]

    for file in macfiles:
        print 'Info: starting simulation with mac file ', file
	make_directory(file)
        os.chdir (file)
        dl_fls = ['*.stdout', '*.stderr', '*.dat']
        for fl in dl_fls:
            delete_files(fl)
        outfile = file + '.stdout'
        errfile = file + '.stderr'
        file_out = open(file + '.stdout', 'w')
        file_err = open(file + '.stderr', 'w')
        cmd = os.path.join(G4MY,'ElecBackScat') + ' ' + os.path.join(G4WORKDIR, rundir, file) + '.mac'
        process = subprocess.Popen(cmd.split(),stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = process.communicate()
        process.wait()
        file_out.write(out)
        file_err.write(err)
        file_out.close()
        file_err.close()

        shutil.move('res.dat', 'res_'+file+'.dat')
        shutil.move('res_summary.dat', 'res_summary_'+file+'.dat')
        os.chdir('..')

def read_env_var(envar):
    if os.environ.get(envar) is None:
	print '''No environment variable %s''' % envar
	sys.exit()
    else:
	return os.environ.get(envar)

def make_directory(dname):
    if not os.path.exists(dname):
	os.mkdir(dname)

def delete_files(param):
    filelist = glob.glob(param)
    for f in filelist:
        try:
            os.remove(f)
        except OSError:
            pass

###______________________________
if __name__ == '__main__':
    run_loop()
