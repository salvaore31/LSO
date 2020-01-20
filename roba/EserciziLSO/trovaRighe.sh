#! /bin/bash

if [ $# -ne 4 ]
then
	echo "devi passare 4 parametri"
	exit 1
fi

if [ ! -d $4 ]
then
	echo "parametro 4 non è una directory"
	exit 1
fi

str1=$1
str2=$2
str3=$3
path=$4
p=$path
for file in $( ls -R $path )
do
	if [ -f $p/$file ]
	then
		for s in $(cat $p/$file | grep "$str1" | grep "$str2" | grep -v "$str3")
		do
			echo "$file $s" > "$path/elencoFile.txt"
		done
	elif [ -d $p/$file ]
	then
		p=$p/$file
	fi
done 
