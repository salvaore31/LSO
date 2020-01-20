#!/bin/bash

if [ $# -ne 3 ]
then
	echo "Passa 3 parametri"
	exit -1
fi


str=$1
n=$3
path=$2
p=$path
for file in $(ls -R $path | grep "\<$str")
do
	if [ -f $p/$file ]
	then
		num=$(grep -o $p/$file "\<$str" | wc -l)
		if [ $num -ge $n ]
		then
			echo "$file $p $num ">>$path/elencoFile.txt
		fi
	elif [ -d $p/$file ]
	then
		$p=$p/$file
	fi
done
