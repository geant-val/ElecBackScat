#!/bin/csh -f

#----------------------------------------------------------------
# Last update: 17-Dec-2012
#
# Script makes ElecBackScatSand production  
#
#----------------------------------------------------------------

mkdir -p $VFEM/ElecBackScatSand/$REFERENCE
cd $VFEM/ElecBackScatSand/$REFERENCE

##  ========== START Production ========== 

# cuts
setenv CUTS '1 um'
setenv STEPMAX '1 cm'

# Physics Lists
set ip = 7
set listPL = (emstandard_opt0 emstandard_opt3 emstandard_opt4 empenelope emlivermore emlowenergy standardSS)
#set ip = 1
#set listPL = (emstandard_opt0 emstandard_opt1 emstandard_opt2 emstandard_opt3 emstandard_opt4 standardSS standardSSM standardWVI standardGS empenelope emlivermore)

# Macro
set im = 14
set listM = (aluminium_00deg aluminium_60deg beryllium_01deg beryllium_61deg carbon_00deg carbon_60deg molybdenum_00deg molybdenum_60deg tantalum_00deg tantalum_60deg titanium_00deg titanium_60deg uranium_00deg uranium_60deg)

#*** Main loop ***
set iPhys = 0
while ($iPhys < $ip)
  @ iPhys++
  setenv PHYSLIST $listPL[$iPhys]
  mkdir -p $PHYSLIST
  cd $PHYSLIST
  rm -f *.out *.dat
  set iMac = 0
  echo "Start production for " $PHYSLIST >! test.out
  while ($iMac < $im)
    @ iMac++
    $G4MY/ElecBackScatSand  $EMSRC/ElecBackScatSand/macro/$listM[$iMac] >>& test.out
  end
  cd ../
end

#  ========== END Production ========== 
