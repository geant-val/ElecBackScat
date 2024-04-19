#!/usr/bin/env python

import os, sys, subprocess

def plot():
    EMSRC = readEnvVar('EMSRC')
    os.chdir(os.path.join(EMSRC,'ElecBackScat','scripts','plot'))
    VFEM = readEnvVar('VFEM')
    REFERENCE = readEnvVar('REFERENCE')

    file_out = open(os.path.join(VFEM,'ElecBackScat',REFERENCE,'p.out'), 'a')
    macrodirs = ['Aluminium', 'Beryllium', 'Carbon', 'Molybdenum', 'Tantalum', 'Titanium', 'Uranium']
    for dr in macrodirs:
        if dr == 'Beryllium':
            angls = ['00deg','15deg','30deg','45deg','60deg','75deg','83deg']
        else:
            angls = ['00deg','15deg','30deg','45deg','60deg','75deg']
        for aan in angls:
            for ic in ['energy','number']:
                cmd = 'python plotpowrat_numfrac.py ' + dr + ' ' + aan + ' ' + ic
                proc = subprocess.Popen(cmd.split(),stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                out, err = proc.communicate()
                proc.wait()
                file_out.write(out)
                file_out.write(err)

    file_out.close()

def readEnvVar(envar):
    if os.environ.get(envar) is None:
        print '''No environment variable %s''' % envar
        sys.exit()
    else:
        return os.environ.get(envar)

###______________________________
if __name__ == '__main__':
    plot()

