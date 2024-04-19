#!/usr/bin/env python

import os, sys, math
from ROOT import gROOT, TROOT, gPad, TCanvas, TLegend, TGraphErrors, kRed
from array import array

def main(): 
    gROOT.Reset()
    gROOT.SetStyle('Plain')
    gROOT.SetBatch(True)

    if len(sys.argv) < 4:
        print '''Usage: python plotpowrat_numfrac.py Aluminium directory inputCoeff'''
        sys.exit()
    else:
        inputMaterial = sys.argv[1]
        directory = sys.argv[2]
        inputCoeff = sys.argv[3]

    EMSRC = readEnvVar('EMSRC')
    VFEM = readEnvVar('VFEM')
    REFERENCE = readEnvVar('REFERENCE')

    mater_dic = {
        'Aluminium':['G4_Al','Al','0.014'],
        'Beryllium':['G4_Be','Be','0.016'],
        'Carbon':['G4_C','C','0.02'],
        'Molybdenum':['G4_Mo','Mo','0.016'],
        'Tantalum':['G4_Ta','Ta','0.012'],
        'Titanium':['G4_Ti','Ti','0.01'],
        'Uranium':['G4_U','U','0.014']
        }
    global exp_data_folder
    exp_data_folder = os.path.join(EMSRC, 'ElecBackScat', 'data')

    inputAngle = directory.replace('deg','')
    if inputAngle == '00':
        inputAngle = inputAngle.replace('00', '0')

    inputFileName = os.path.join(exp_data_folder, mater_dic[inputMaterial][1] + '_')
    inputFileName = setInFileName(inputCoeff, inputFileName)
    stat = 0.0025
    tt_err = float(mater_dic[inputMaterial][2]) + stat

    new_angle = inputAngle

    if(inputMaterial == 'Beryllium'):
        if inputAngle == '83':
            inputAngle =  str(float(inputAngle) + 0.5)
        else:
            inputAngle =  str(int(inputAngle) + 1)
    title = 'Target material: ' + mater_dic[inputMaterial][0] + ', incident angle: ' + inputAngle + ' deg'

    c = TCanvas('c','Histogram Drawing Options',200,10,700,500)

    ymin = 0.
    ymax = 1.
    ymaxsim = array('d',[])
    h0 = gPad.DrawFrame(0.,ymin,1.1,ymax,title)
    if(inputCoeff=='energy'):
        h0.GetYaxis().SetTitle('Backscattered ' + inputCoeff + ' coefficient')
    if(inputCoeff=='number'):
        h0.GetYaxis().SetTitle('Backscattered electron ' + inputCoeff + ' coefficient')
    h0.GetXaxis().SetTitle('Energy (MeV)')
    h0.GetYaxis().SetTitleOffset(1.4)
    h0.Draw('AXIS SAME')
    lg = makeLegend()

    energyExp = array( 'd', [] )
    valueExp = array( 'd', [] )
    energyExp_error = array( 'd', [] )
    valueExp_error = array( 'd', [] )
    
    ymaxdt = 0.
    if os.path.isfile(inputFileName):
        energyExp, valueExp, energyExp_error, valueExp_error = readData(inputAngle, inputMaterial, inputCoeff, inputFileName, tt_err)

        ymaxdt = max(valueExp)
        dataExp = makeGraphErrorsData(energyExp, valueExp, energyExp_error, valueExp_error, lg)
        dataExp.Draw('P SAME')

    phlend = 'GS', 'Opt0', 'Opt3', 'SS'
    for phl in phlend:
        filename = 'res_' + directory + '_emstandard' +  phl + '.dat'

        globals()['energy' + phl] = array( 'd', [] )
        globals()['angle' + phl] = array( 'd', [] )
        globals()['eneCoeff' + phl] = array( 'd', [] )
        globals()['eneCoeffErr' + phl] = array( 'd', [] )
        globals()['numCoeff' + phl] = array( 'd', [] )
        globals()['numCoeffErr' + phl] = array( 'd', [] )

        sim_data_folder = os.path.join(VFEM, 'ElecBackScat', REFERENCE, inputMaterial, os.path.splitext(filename.replace('res_',''))[0])
        inputFileName = os.path.join(sim_data_folder,filename)
        lines = readLinesFile(inputFileName)
        for line in lines:
            globals()['energy' + phl].append(float(line.split() [1]))
            globals()['angle' + phl].append(float(line.split() [2]))
            globals()['eneCoeff' + phl].append(float(line.split() [3]))
            globals()['eneCoeffErr' + phl].append(float(line.split() [4]))
            globals()['numCoeff' + phl].append(float(line.split() [5]))
            globals()['numCoeffErr' + phl].append(float(line.split() [6]))
        
        en_err = array('d', len(globals()['energy' + phl])*[0.])

        makeGraphErrorsSim(phl, title, globals()['energy' + phl], globals()[inputCoeff[:3] + 'Coeff' + phl], en_err, globals()[inputCoeff[:3] + 'CoeffErr' + phl],lg)

        ymaxsim.append(max(globals()[inputCoeff[:3] + 'Coeff' + phl]))

    ymax = max(ymaxdt,max(ymaxsim))
    h0.GetYaxis().SetRangeUser(0.,1.2*ymax)
    for phll in phlend:
        globals()['myplot' + phll].Draw('L SAME')

    c.cd()
    lg.Draw()

    outputFig = inputMaterial + '_' + inputAngle.replace('.','_') + 'deg_' + inputCoeff[:3] + 'coef'
    c.Print(os.path.join(VFEM,'ElecBackScat',REFERENCE,outputFig + '.png'))

def readEnvVar(envar):
    if os.environ.get(envar) is None:
        print '''No environment variable %s''' % envar
        sys.exit()
    else:
        return os.environ.get(envar)

def makeDirectory(dname):
    if not os.path.exists(dname):
        os.mkdir(dname)

def readLinesFile(filename):
    try:
        infile = open(filename, 'r')
    except IOError:
        print 'Input file <',filename, '> does not exist! Exit'
        sys.exit(2)
    return infile.readlines()

def setInFileName(incoef, inFilNm):
    inFilNam = inFilNm + incoef + '.tsv'
    return inFilNam

def makeLegend():
    legend = TLegend(0.84,0.68,0.99,0.93)
    legend.SetTextFont(42)
    legend.SetTextSize(0.04)
    return legend

def readData(iangle, imaterial, icoeff, infname, tot_error):
    Energy, Value, Energy_err, Value_err = array( 'd', [] ), array( 'd', [] ), array( 'd', [] ), array( 'd', [] )

    lines = readLinesFile(infname)
    print iangle
    idx = lines[0].split().index(iangle)

    for line in lines[1:]:
        if line.startswith('Exp'):
            Energy.append(float(line.split()[1]))
            Value.append(float(line.split()[idx]))
            Energy_err.append(0.)
            Value_err.append(tot_error*float(line.split()[idx]))
            
    return Energy, Value, Energy_err, Value_err

def makeGraphErrorsData(enrg, val ,enrg_err, val_err, leg): 
    dataGE = TGraphErrors(len(enrg), enrg, val ,enrg_err, val_err)
    dataGE.SetMarkerColor(1)
    dataGE.SetLineWidth(2)
    dataGE.SetMarkerStyle(20) 
    dataGE.SetMarkerSize(1.) 
    leg.AddEntry(dataGE,'Data','p')
    return dataGE

def makeGraphErrorsSim(prm, tit, en,  parcoef, enerr, parcoeferr, leg):
    globals()['myplot' + prm] = TGraphErrors(len(en), en, parcoef, enerr, parcoeferr)
    globals()['myplot' + prm].SetMarkerStyle(11)
    globals()['myplot' + prm].SetTitle(tit)
#    globals()['myplot' + prm].SetLineWidth(2)
    if prm == 'GS':
        globals()['myplot' + prm].SetMarkerColor(3)
        globals()['myplot' + prm].SetLineColor(3)
    elif prm == 'Opt0':
        globals()['myplot' + prm].SetMarkerColor(43)
        globals()['myplot' + prm].SetLineColor(43)
        legtitle = 'emstandard_opt'
    elif prm == 'Opt3':
        globals()['myplot' + prm].SetMarkerColor(4)
        globals()['myplot' + prm].SetLineColor(4)
    elif prm == 'SS':
        globals()['myplot' + prm].SetMarkerColor(kRed)
        globals()['myplot' + prm].SetLineColor(kRed)
    leg.AddEntry(globals()['myplot' + prm],prm,'l')

###______________________________
if __name__ == '__main__':
    main()
