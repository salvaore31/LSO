#! /bin/bash

if [ $# -ne 4 ]
then
	echo "Passa 4 parametri"
	exit 1
fi

if [ ! -d $1 -o ! -d $2 ]
then
	echo "Parametro 1 e 2 devono essere directory"
	exit 1
fi

path1=$1
path2=$2
str1=$3
str2=$4

for file in $(ls $path1 | grep "^$str2")
do
	if [ -f $path1/$file ]
	then	
		if [ $(cat $path1/$file | tail -1 | grep "$str1$" | wc -l) -ge 1 ]
		then
			cat $path1/$file | sed "s/ //g" >> $path2/$file
		fi
	fi
done
