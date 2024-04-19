#!/bin/bash

#export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${G4WORKDIR}/tmp/Linux-g++/ElecBackScatSand/"
G4WORKDIR=$PWD
echo "Working dir: " $PWD
echo "Macro dir: " $1
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

if [ $? == 0 ] ; then

   macfiles=`ls *mac | sed 's/\.mac//'`
   rm -f *.stdout *.stderr *.dat

   for file in $macfiles ; do
       
       echo "Info: starting simulation in "${PWD}" with mac file "$file

       ( $G4WORKDIR/ElecBackScatSand $file > ${file}.stdout ) 2> ${file}.stderr 

       read outfold _ < res.dat
       echo "Creating output directory: " $G4WORKDIR/results/$outfold/$file
       mkdir -p $G4WORKDIR/results/$outfold
       mkdir -p $G4WORKDIR/results/$outfold/$file
       mv -f res.dat $G4WORKDIR/results/$outfold/$file/res_$file.dat
       mv -f res_summary.dat $G4WORKDIR/results/$outfold/$file/res_summary_$file.dat
       mv -f *.std* $G4WORKDIR/results/$outfold/$file/ 
       mv -f physlist.info $G4WORKDIR/results/$outfold/$file/     

   done

   popd

fi

echo "Post-exec processing..."
cd $G4WORKDIR
