#!/bin/bash

if [ $# -ne 3 ]
then echo "devono essere 3 parametri"
exit 1
fi

if [ ! -d $2 ]
then echo "il secondo deve essere una dir"
exit 1
fi

str=$1
path=$2
n=$3
for file in $( ls -R $path | grep -v -i "[aeiou]\{$n,\}")
do	if [ -f $path/$file ]
	then	
		num=$(grep -c "$str\>" $path/$file)
		if [ $num -lt $n ]
		then
			echo "$file $path $num" >> $2/elencoFile.txt
		fi
	elif [ -d $path/$file ]
	then
		path=$path/$file
	fi		
done

echo "Script fatto"
