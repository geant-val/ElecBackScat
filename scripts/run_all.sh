#!/bin/bash
#standard macro to run the simulation on all the materials and angles
echo "--> Run Alluminium"
source run.sh ../macro/Aluminium/  
echo "--> Run Beryllium"
source run.sh ../macro/Beryllium/ 
echo "--> Run Carbonium" 
source run.sh ../macro/Carbon/  
echo "--> Run Molybdenum"
source run.sh ../macro/Molybdenum/  
echo "--> Run Tantalum"
source run.sh ../macro/Tantalum/
echo "--> Run Titanium"
source run.sh ../macro/Titanium/
echo "--> Run Uranium"  
source run.sh ../macro/Uranium/
echo "--> Run low energy Al"
source run_loop.sh ../macro/AlLoop/
