#! /bin/bash

if [ $# -ne 4 ]
then
	echo "devi passare 4 parametri"
	exit 1
fi

if [ ! -d $1 -o ! -d $2 ]
then
	echo "1 e 2 devono essere directory"
	exit 1
fi

path1=$1
path2=$2
str1=$3
str2=$4

for file in $(ls $path1)
do
	if [ -f $path1/$file ]
	then
		cat $path1/$file | sed "/$str1/p" | sed "s/$str2/$str1/g" > $path2/$file
		cat $path2/$file | wc -l >> $path2/$file
	fi
done
