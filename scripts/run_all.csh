#!/bin/csh -f

#standard macro to run the simulation on all the materials and angles
echo "--> Run Alluminium"
source run.csh ../macro/Aluminium/  
echo "--> Run Beryllium"
source run.csh ../macro/Beryllium/ 
echo "--> Run Carbonium" 
source run.csh ../macro/Carbon/  
echo "--> Run Molybdenum"
source run.csh ../macro/Molybdenum/  
echo "--> Run Tantalum"
source run.csh ../macro/Tantalum/
echo "--> Run Titanium"
source run.csh ../macro/Titanium/
echo "--> Run Uranium"  
source run.csh ../macro/Uranium/
echo "--> Run low energy Al"
source run_loop.sh ../macro/AlLoop/
