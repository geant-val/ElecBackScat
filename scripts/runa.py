#!/usr/bin/env python

import os, sys
import glob, subprocess, shutil

def runa():
    rundir = readEnvVar('EBACKSCT')
    print rundir
    VFEM = readEnvVar('VFEM')
    print VFEM
    makeDirectory(os.path.join(VFEM,'ElecBackScat'))
    os.chdir(os.path.join(VFEM,'ElecBackScat'))

    REFERENCE = readEnvVar('REFERENCE')
    makeDirectory(REFERENCE)
             
    os.chdir (REFERENCE)

    G4BWORK = readEnvVar('G4BWORK')
    EMSRC   = readEnvVar('EMSRC')
    G4WORKDIR = os.path.join(EMSRC,'ElecBackScat', 'macro')

    makeDirectory(rundir)
    os.chdir (rundir)

    macfiles = [f.replace('.mac','') for f in os.listdir(os.path.join(G4WORKDIR, rundir)) if f.endswith('.mac')]

    for fl in macfiles:
        makeDirectory(fl)
        os.chdir (fl)
        dl_fls = ['*.stdout*', '*.stderr*', '*.dat*', '*.log*']
        for fls in dl_fls:
            deleteFiles(fls)
        outfile = fl + '.stdout'
        file_out = open(fl + '.log', 'w')
        cmd = os.path.join(G4BWORK,'ElecBackScat') + ' ' + os.path.join(G4WORKDIR, rundir, fl) + '.mac'
        process = subprocess.Popen(cmd.split(),stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = process.communicate()
        process.wait()
        file_out.write(out)
        file_out.write(err)
        file_out.close()

        shutil.move('res.dat', 'res_'+fl+'.dat')
        shutil.move('res_summary.dat', 'res_summary_'+fl+'.dat')
        os.chdir('..')
    os.chdir('..')

def readEnvVar(envar):
    if os.environ.get(envar) is None:
        print '''No environment variable %s''' % envar
        sys.exit()
    else:
        return os.environ.get(envar)

def makeDirectory(dname):
    if not os.path.exists(dname):
	os.mkdir(dname)

def deleteFiles(param):
    filelist = glob.glob(param)
    for f in filelist:
        try:
            os.remove(f)
        except OSError:
            pass

###______________________________
if __name__ == '__main__':
    runa()
