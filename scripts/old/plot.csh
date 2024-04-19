#!/bin/bash

if [ $# == 0 ] ; then
 
  echo "Error: Specify directory containing simulation data."

  exit -1

fi

rundir=$1

if [ ! -d $rundir ] ; then
 
  echo "Error: Specify directory containing simulation data."

  exit -1

fi


gnuplot <<EOF
#
################################################
# Common definitions
################################################
#
set terminal postscript eps enhanced "Times-Roman" 16
#
set xlabel "{/Times-Italic E_i} (MeV)" 0.0,0.0
set ylabel "{/Symbol-Oblique h}_{{N}}" 1.0,0.0
#
#
set border 31 lw 1
set lmargin 7
#
set key top right
set key samplen 1
set key width -1
#
set bars small
#
set ytics 0.1
#
ifac1=0.58
ifac2=0.40
#
set xrange [0:1.2]
#
dir="$rundir"
#
#
################################################
# Plot 1: 0 deg, charge backscatter
################################################
#
set output dir."/chargebsc_00deg.eps"
#
set size 1.0*ifac1,2.0*ifac2
#
set yrange [0.0:0.6]
#
set label 2 "Mo" at 1.06,0.305
set label 3 "Ti" at 1.06,0.18    
set label 4 "Al" at 1.06,0.1
set label 5 "C" at 1.035,0.043
set label 6 "Be" at 1.06,0.015
set label 7 "Ta" at 1.06,0.44 
set label 8 "U" at 1.06,0.49 
#
plot "data/chargebsc/beryllium.dat" u 1:2 not w points ps 1.25 pt 7 lt 1,\
     dir."/beryllium_01deg.dat" u 1:5 not  w l lt 1,\
     dir."/beryllium_01deg.dat" u 1:5 not w p ps 1.3 pt 71 lw 1,\
\
     "data/chargebsc/carbon.dat" u 1:2 not w points ps 1.75 pt 9 lt 1,\
     dir."/carbon_00deg.dat" u 1:5 not  w l lt 1,\
     dir."/carbon_00deg.dat" u 1:5 not w p ps 1.6 pt 72 lw 1,\
\
     "data/chargebsc/aluminium.dat" u 1:2 not w points ps 1.15 pt 63 lt 1,\
     dir."/aluminium_00deg.dat" u 1:5 not  w l lt 1,\
     dir."/aluminium_00deg.dat" u 1:5 not w p ps 1.7 pt 74 lw 1,\
\
     "data/chargebsc/titanium.dat" u 1:2 not w points ps 1.75 pt 11 lt 1,\
     dir."/titanium_00deg.dat" u 1:5 not  w l lt 1,\
     dir."/titanium_00deg.dat" u 1:5 not w p ps 1.6 pt 73 lw 1,\
\
     "data/chargebsc/molybdenum.dat" u 1:2 not w points ps 1.35 pt 5 lt 1,\
     dir."/molybdenum_00deg.dat" u 1:5 not  w l lt 1,\
     dir."/molybdenum_00deg.dat" u 1:5 not w p ps 1.2 pt 70 lw 1,\
\
     "data/chargebsc/tantalum.dat" u 1:2 not w points ps 1.75 pt 9 lt 1,\
     dir."/tantalum_00deg.dat" u 1:5 not  w l lt 1,\
     dir."/tantalum_00deg.dat" u 1:5 not w p ps 1.6 pt 72 lw 1,\
\
     "data/chargebsc/uranium.dat" u 1:2 not w points ps 1.4 pt 7 lt 1,\
     dir."/uranium_00deg.dat" u 1:5 not  w l lt 1,\
     dir."/uranium_00deg.dat" u 1:5 not w p  ps 1.3 pt 71 lw 1
#
#
################################################
# Plot 2: 60 deg, charge backscatter
################################################
#
set output dir."/chargebsc_60deg.eps"
#
set size 1.0*ifac1,2.0*ifac2
#
set yrange [0.1:0.75]
#
set label 2 "Mo" at 1.06,0.535
set label 3 "Ti" at 1.06,0.435
set label 4 "Al" at 1.06,0.35
set label 5 "C" at 1.04,0.24
set label 6 "Be" at 1.06,0.19
set label 7 "Ta" at 1.06,0.62 
set label 8 "U" at 1.06,0.66
#
plot "data/chargebsc/beryllium.dat" u 1:3 not w points ps 1.4 pt 7 lt 1,\
     dir."/beryllium_61deg.dat" u 1:5 not  w l lt 1,\
     dir."/beryllium_61deg.dat" u 1:5 not w p ps 1.3 pt 71 lw 1,\
\
      "data/chargebsc/carbon.dat" u 1:3 not w points ps 1.75 pt 9 lt 1,\
     dir."/carbon_60deg.dat" u 1:5 not  w l lt 1,\
     dir."/carbon_60deg.dat" u 1:5 not w p ps 1.6 pt 72 lw 1,\
\
     "data/chargebsc/aluminium.dat" u 1:3 not w points ps 1.15 pt 63 lt 1,\
     dir."/aluminium_60deg.dat" u 1:5 not  w l lt 1,\
     dir."/aluminium_60deg.dat" u 1:5 not w p ps 1.7 pt 74 lw 1,\
\
     "data/chargebsc/titanium.dat" u 1:3 not w points ps 1.75 pt 11 lt 1,\
     dir."/titanium_60deg.dat" u 1:5 not  w l lt 1,\
     dir."/titanium_60deg.dat" u 1:5 not w p ps 1.6 pt 73 lw 1,\
\
     "data/chargebsc/molybdenum.dat" u 1:3 not w points ps 1.35 pt 5 lt 1,\
     dir."/molybdenum_60deg.dat" u 1:5 not   w l lt 1,\
     dir."/molybdenum_60deg.dat" u 1:5 not w p ps 1.2 pt 70 lw 1,\
\
     "data/chargebsc/tantalum.dat" u 1:3 not w points ps 1.75 pt 9 lt 1,\
     dir."/tantalum_60deg.dat" u 1:5 not   w l lt 1,\
     dir."/tantalum_60deg.dat" u 1:5 not w p ps 1.6 pt 72 lw 1,\
\
     "data/chargebsc/uranium.dat" u 1:3 not w points ps 1.4 pt 7 lt 1,\
     dir."/uranium_60deg.dat" u 1:5 not   w l lt 1,\
     dir."/uranium_60deg.dat" u 1:5 not w p  ps 1.3 pt 71 lw 1
#
#
################################################
# Common definitions
################################################
#
set xlabel "{/Times-Italic E_i} (MeV)" 0.0,0.0
set ylabel "{/Symbol-Oblique h}_{{E}}" 1.0,0.0
#
#
################################################
# Plot 3: 0 deg, energy backscatter
################################################
#
set output dir."/energybsc_00deg.eps"
#
set size 1.0*ifac1,2.0*ifac2
#
set yrange [0.0:0.45]
#
unset label 8
unset label 6
set label 2 "Mo" at 1.06,0.19
set label 3 "Ti" at 1.06,0.1
set label 4 "Al" at 1.06,0.045
set label 5 "C" at 1.04,0.015
set label 7 "Ta" at 1.06,0.29
#
plot "data/energybsc/carbon.dat" u 1:2 not w points ps 1.75 pt 9 lt 1,\
     dir."/carbon_00deg.dat" u 1:3 not  w l lt 1,\
     dir."/carbon_00deg.dat" u 1:3 not w p ps 1.6 pt 72 lw 1,\
\
     "data/energybsc/aluminium.dat" u 1:2 not w points ps 1.15 pt 63 lt 1,\
     dir."/aluminium_00deg.dat" u 1:3 not  w l lt 1,\
     dir."/aluminium_00deg.dat" u 1:3 not w p ps 1.7 pt 74 lw 1,\
\
     "data/energybsc/titanium.dat" u 1:2 not w points ps 1.75 pt 11 lt 1,\
     dir."/titanium_00deg.dat" u 1:3 not  w l lt 1,\
     dir."/titanium_00deg.dat" u 1:3 not w p ps 1.6 pt 73 lw 1,\
\
     "data/energybsc/molybdenum.dat" u 1:2 not w points ps 1.35 pt 5 lt 1,\
     dir."/molybdenum_00deg.dat" u 1:3 not  w l lt 1,\
     dir."/molybdenum_00deg.dat" u 1:3 not w p ps 1.2 pt 70 lw 1,\
\
     "data/energybsc/tantalum.dat" u 1:2 not w points ps 1.75 pt 9 lt 1,\
     dir."/tantalum_00deg.dat" u 1:3 not  w l lt 1,\
     dir."/tantalum_00deg.dat" u 1:3 not w p ps 1.6 pt 72 lw 1
#
#
################################################
# Plot 4: 60 deg, energy backscatter
################################################
#
set output dir."/energybsc_60deg.eps"
#
set size 1.0*ifac1,2.0*ifac2
#
set yrange [0.1:0.6]
#
set label 2 "Mo" at 1.06,0.39
set label 3 "Ti" at 1.06,0.295
set label 4 "Al" at 1.06,0.21
set label 5 "C" at 1.04,0.125
set label 6 "Be" at 1.06,0.09
set label 7 "Ta" at 1.06,0.48
#
plot "data/energybsc/beryllium.dat" u 1:3 not w points ps 1.4 pt 7 lt 1,\
     dir."/beryllium_61deg.dat" u 1:3 not  w l lt 1,\
     dir."/beryllium_61deg.dat" u 1:3 not w p ps 1.3 pt 71 lw 1,\
\
      "data/energybsc/carbon.dat" u 1:3 not w points ps 1.75 pt 9 lt 1,\
     dir."/carbon_60deg.dat" u 1:3 not  w l lt 1,\
     dir."/carbon_60deg.dat" u 1:3 not w p ps 1.6 pt 72 lw 1,\
\
     "data/energybsc/aluminium.dat" u 1:3 not w points ps 1.15 pt 63 lt 1,\
     dir."/aluminium_60deg.dat" u 1:3 not  w l lt 1,\
     dir."/aluminium_60deg.dat" u 1:3 not w p ps 1.7 pt 74 lw 1,\
\
     "data/energybsc/titanium.dat" u 1:3 not w points ps 1.75 pt 11 lt 1,\
     dir."/titanium_60deg.dat" u 1:3 not  w l lt 1,\
     dir."/titanium_60deg.dat" u 1:3 not w p ps 1.6 pt 73 lw 1,\
\
     "data/energybsc/molybdenum.dat" u 1:3 not w points ps 1.35 pt 5 lt 1,\
     dir."/molybdenum_60deg.dat" u 1:3 not   w l lt 1,\
     dir."/molybdenum_60deg.dat" u 1:3 not w p ps 1.2 pt 70 lw 1,\
\
     "data/energybsc/tantalum.dat" u 1:3 not w points ps 1.75 pt 9 lt 1,\
     dir."/tantalum_60deg.dat" u 1:3 not   w l lt 1,\
     dir."/tantalum_60deg.dat" u 1:3 not w p ps 1.6 pt 72 lw 1
#
#
#EOF 
