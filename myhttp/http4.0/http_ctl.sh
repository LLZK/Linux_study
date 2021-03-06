#!/bin/bash

ROOT_PATH=$(pwd)
CONF=${ROOT_PATH}/conf/http.conf
BIN=${ROOT_PATH}/httpd
PID=${ROOT_PATH}/http.pid

proc=`basename $0`
function usage(){
	printf "%s [start(-s)][stop(-t)][restart[-rs]]\n" $proc
}

function http_start(){
	[[ -f $PID ]] && {
		printf "httpd is exist !! pid is $(cat $PID)\n"
		return
	}

	ip=$(grep -E '^IP:' $CONF | awk -F: '{print $2}')
	port=$(grep -E '^PORT:' $CONF | awk -F: '{print $2}')

	$BIN $ip $port
	pidof $(basename $BIN) > $PID
	printf "start done...pid is $(cat $PID)\n"
}

function http_stop(){
	[[ ! -f $PID ]] && {
		printf "httpd is not exist!!\n"
		return
	}
	pid=$(cat $PID)
	kill -9 $pid
	rm -f $PID
	printf "stop done...\n"
}

[[ $# -ne 1 ]] && {
	usage
	exit 1
	
}

case $1 in 
	start | -s)
		http_start
	;;
	stop | -t)
		http_stop
	;;
	restart| -rt)
		http_stop
		http_start
	;;
	*)
		usage
		exit 2
	;;
esac


