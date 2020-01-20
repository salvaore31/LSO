#!/bin/bash

pu=$(ps -u $USER | wc -l )
echo "Il numero processi di $USER  è $pu"
for use in $(cat /etc/passwd | grep -v $USER | sed -e "s/^.*x://g" -e "s/:.*$//g" )
do

	n=$(ps -u $use | wc -l)
	echo "Il numero processi di $use  è $n"	
	if [ $n -gt $pu ]
	then
		u=$(cat /etc/passwd | grep ":$use:" | sed "s/:x:.*$//g")
		echo "$u $n" >> "moniot/$u.txt"
	fi
done
