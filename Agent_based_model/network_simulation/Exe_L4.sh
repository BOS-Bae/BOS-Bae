#!/bin/bash

for ((N=4; N<$1; N++))
do
	mod=$((N % 2))
	if [ $mod -eq 0 ]
		then
			./Exe_L4_ABM_histogram $N 4 100000 >> Execution_L4.dat
		fi
done
