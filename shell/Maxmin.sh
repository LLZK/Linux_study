#!/bin/bash
sum=0

if [ $# -lt 3 ] ;then
	echo "PLease input 3 numbers"
else
	min=$1
	max=$1
	for i in $@
	do
		if [ $i -lt $min ] ;then
			min=$i
		elif [ $i -gt $max ] ;then
			max=$i
		else
			:
	    fi
		let sum+=$i
	done
	printf "max is %d\n" $max
	printf "min is %d\n" $min
	#echo $sum
	arr=$(($sum/$#)) 
	echo "ibase=10;scale=2;$sum/$#" | bc

fi


