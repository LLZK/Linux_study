#!/bin/bash

sum=0
i=$1


first=1
second=1
third=2
if [ $1 -eq 1 ] || [ $1 -eq 2 ];then
	sum=1
	echo $sum
	exit 1
fi


while [ $i -ge 2 ]
do
	((third=first+second))
	#echo $third
	first=$second
	#echo $first
	second=$third
	#echo $second
	let i--
done

echo $third


#function fib
#{
#	if [ $1 -eq 0 ];then
#		let sum+=0
#	elif [ $1 -eq 2 ];then
#		let sum+=1
#	else
#		sum=$(($(fib $1-1)+$(fib $1-2)))
#	fi
#}
#
#fib $1
#
#echo $sum





