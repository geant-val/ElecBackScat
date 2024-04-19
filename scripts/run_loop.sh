#!/bin/bash

# Script to run macros with more points at low energies (for the C. Joy plot at low energy)

G4WORKDIR=$PWD
echo "Working dir: " $PWD
echo "Macro file dir: " $1
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${G4WORKDIR}"

if [ $# == 0 ] ; then
 
  echo "Error: Specify directory containing .mac files."

  exit -1

fi

rundir=$1

if [ ! -d $rundir ] ; then
 
  echo "Error: Specify directory containing .mac files."

  exit -1

fi

pushd $rundir
for macfile in 00deg_emstandatdGS.mac 00deg_emstandatdOpt0.mac 00deg_emstandatdOpt3.mac 00deg_emstandatdSS.mac
do
  file=`echo $macfile | sed 's/\.mac//'`
  echo "Info: starting simulation in "${PWD}" with mac file "$macfile
  ( $G4WORKDIR/ElecBackScatSand $file >> $file.stdout ) 2> $file.stderr 
       
       read outfold _ < res.dat
       echo "Creating output directory: " $G4WORKDIR/results/$outfold/$file
       mkdir -p $G4WORKDIR/results/$outfold
       mkdir -p $G4WORKDIR/results/$outfold/$file
       mv -f res.dat $G4WORKDIR/results/$outfold/$file/res_$file.dat
       mv -f res_summary.dat $G4WORKDIR/results/$outfold/$file/res_summary_$file.dat
       mv -f *.std* $G4WORKDIR/results/$outfold/$file/ 
       mv -f physlist.info $G4WORKDIR/results/$outfold/$file/      
done

echo "Post-exec processing..."
cd $G4WORKDIR
