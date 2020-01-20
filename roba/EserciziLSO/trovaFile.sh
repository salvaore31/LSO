#!/bin/bash

if [ $# -ne 3 ]
then echo "devono essere 3 parametri"
exit -1
fi

if [ ! -d $2 ]
then echo "il secondo deve essere una directory"
exit -1
fi

str=$1
path=$2
num=$3
p=$path
for file in $( ls  -R $path )
do	
	if [ $(echo "$file" | grep -e "\<$str" | wc -l) -ge 1 ]
	then
		if [ -f $p/$file ]
		then
			n=$(grep -o  "\<$str"  $p/$file | wc -l)
	 		if [  $n -ge $num ]
			then	
				echo "$file $p $n ">>$path/elencoFile.txt
			fi
		fi
	fi
	if [ -d $p/$file ]
	then
			
		p=$p/$file
	fi
done
