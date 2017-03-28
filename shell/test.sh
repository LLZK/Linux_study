#!/bin/bash

bar=''
i=0
f=('|' '/' '-' '\\')
while [ $i -le 100 ]
do
	let index=i%4
	#printf "[%d%%][\e[47;46;1m%-100s\e[0m]][%c]\r" $i "$bar" "${f[$index]}"
	printf "[%d%%][\033[47;46;5m%-100s\033[0m][%c]\r" $i "$bar" "${f[$index]}"
	bar=${bar}'#'
	let i++
	sleep 0.1
done
printf "\n"


#if :;then
#	:
#else
#	:
#fi
