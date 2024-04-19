#!/usr/bin/env python

import sys
import os
import json
from gts.utils import getJSON

idx = 0

# def getIsotopeNumber(isotope):
#   r = 0
#   idx = 0
#   while True:
#     try:
#       r = int(isotope[idx:])
#       break
#     except:
#       idx += 1
#   return r

def dump(x, y, angle, target):
    global idx

    for i in xrange(len(x)):
        binlow.append(x[i] * 0.95)
        binhigh.append(x[i] * 1.05)
    s = getJSON({}, "chart",
                      inspireId=-12,
                      testName="experiment",
                      mctool_name="experiment",
                      mctool_model="experiment",
                      mctool_version="experiment",
                      observableName="energy deposition",
                      targetName=target,
                      beamParticle="proton",
                      beamEnergies=[float(energy)],
                      secondaryParticle="None",
                      parameters=parameters,
                      title="",
                      xAxisName="E (MeV/n)",
                      yAxisName="1/N",
                      xValues=x,
                      yValues=y,
                      )
    json.dump(s, open(target + "_" + str(energy) + "_" + str(idx) + ".json", 'w'), indent=2)
    idx += 1
fn = sys.argv[1]

fh = open(fn, 'r')
lines = [x.strip() for x in fh.readlines() if not x.startswith("#")]
fh.close()

target = fn.split("_")[0]
if "energy" in fn:
    observable = "Backscattered energy coefficient"
else:
    observable = "Backscattered electron number coefficient"

header = {}

_l = lines[0].split()
for itemid in xrange(len(_l)):
    try:
        if "." in _l[itemid]:
            a = float(_l[itemid])
        else:
            a = int(_l[itemid])
        header[a] = itemid
    except:
        pass

print observable
for angle in header:
    x = []
    y = []
    print "angle", angle
    parameters = [
                    {"names": "angle", "values": "{0}".format(angle)}
                ]
    for line in reversed(lines[1:]):
        _l = line.split()
        if not line or _l[0] == "Theory":
            continue
        _y = float(_l[header[angle]])
        if _y == 0:
            continue
        x.append(float(_l[1]))
        y.append(_y)
    if not y:
        continue
    s = getJSON({}, "chart",
                      inspireId=-14,
                      testName="experiment",
                      mctool_name="experiment",
                      mctool_model="experiment",
                      mctool_version="experiment",
                      observableName=observable,
                      targetName=target,
                      beamParticle="e-",
                      beamEnergies=x,
                      secondaryParticle="None",
                      parameters=parameters,
                      title=observable,
                      xAxisName="E, MeV",
                      yAxisName=observable,
                      xValues=x,
                      yValues=y,
                      )
    json.dump(s, open("{0}_{1}_{2}_.json".format(target, angle, observable), 'w'), indent=2)