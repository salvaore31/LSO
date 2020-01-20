#!/bin/bash

if [ $# -ne 4 ]
then echo "devono essere 4 parametri"
exit 1
fi

user=$1
dim1=$2
dim2=$3
nomefile=$4
for pid in $(ps -u $user -e -o "pid")
do
	n=$(ps -u $user -o ppid=$pid | wc -l )
	if [ $n -ge $dim1 -a $n -le $dim2 ]
	then
		echo $pid > $4
	fi
done
echo "script fatto"
