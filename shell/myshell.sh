#!/bin/bash
#sum=0
#
#if [ $# -lt 3 ]then;
#	echo 'PLease input 3 numbers'
#else
#	min=$1
#	max=$1
#	for i in $@
#	do
#		if [ $i -lt $min ]then;
#			min=$i
#		elif [ $i -gt $max ]then;
#			max=$i
#		let sum+=i
#	    fi
#	done
#	printf "max is %d\n" $max
#	printf "min is %d\n" $min
#	printf "arr is %2f\n" let (max+min)/2
#
#fi
#


sum=0

function fun
{
	if [ $1 -eq 0 ];then
		echo $sum
	else
		let sum+=$1
		fun $(($1-1))
	fi
}

fun $1
#echo $sum
