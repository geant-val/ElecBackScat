#!/bin/bash

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${G4WORKDIR}/tmp/Linux-g++/ElecBackScatSand/"

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

       ( $G4WORKDIR/bin/Linux-g++/ElecBackScatSand $file > ${file}.stdout ) 2> ${file}.stderr 

   done

   popd

fi

