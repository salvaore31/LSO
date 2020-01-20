#!/bin/bash

if [ $# -ne 7 ]
then
echo "Passa 7 parametri"
fi

if [ ! -d $1 -o ! -d $2 -o ! -d $3 ]
then
echo "i primi 3 parametri devono essere dir"
fi

path1=$1
path2=$2
path3=$3
str1=$4
str2=$5
n1=$6
n2=$7
p=$path1
m=$(($n2-1))
for file in $(ls -R $path1)
do
	if [ -f $p/$file ]
	then	
		n=$(cat $p/$file | grep -e "^$str1.*$str2$" -e "$str2\{0,$m\}" | wc -l)
		if [ $n -ge $n1 ]
		then
			cp $p/$file $path2
		else
			cp $p/$file $path3
		fi
	elif [ -d $p/$file ]
	then
		p=$p/$file		
	fi
done
