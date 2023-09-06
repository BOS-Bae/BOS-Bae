#!/bin/bash

T=0.6500
for ((i=0; i<40; i++))
do
	if [ "$(echo "$T > 0.7300" | bc -l)" -eq 1 ] && [ "$(echo "$T < 0.7600" | bc -l)" -eq 1 ]
	then
		T=$(echo "$T + 0.001" | bc -l)
		T=$(printf "%.4f" $T)
	else
		T=$(echo "$T + 0.01" | bc -l)
		T=$(printf "%.4f" $T)
	fi
	julia Baxter_Wu.jl $1 1000000 1 $T > ./dat_Baxter_Wu/L$1/L$1T$T.dat
done
