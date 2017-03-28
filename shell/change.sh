#!/bin/bash


while read line
do
	typeset -u b
	b=${line:3:3}
	a=${line:0-3:3}
	c=${line:0:3}
	#echo $b
	echo ${a}${b}${c}
done <1.txt
