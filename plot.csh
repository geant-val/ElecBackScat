#!/bin/csh -f
#trace on
#  
#  Analize testing suite results
#  V.Ivanchenko 2018
#
#  This script should be started from directory 
#  with results of run
#
gunzip -q */*/*.gz *.gz
python ${EMSRC}/ElecBackScat/scripts/plot/plot.py
gzip -q */*/*.* p.out

# end
