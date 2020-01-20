#! /bin/bash

if [ $# -ne 5 ]
then
	echo "Devi passare 5 parametri"
	exit 1
fi

if [ ! -d $1 -o ! -d $2 ]
then
	echo "Parametro 1 e 2 devono essere directory"
	exit 1
fi

path1=$1
path2=$2
strn1=$3
strn2=$4
n=$5

for file in $(ls $path1)
do
	if [ -f $path1/$file ]
	then
		num=$(cat $path1/$file | grep "^$strn1.*$strn2" | wc -l)
		if [ $num -ge $n ]
		then 
			cat $path1/$file | grep "^$strn1" | sed "s/$strn2/$strn1/g" > $path2/$file
		fi
	fi
done
