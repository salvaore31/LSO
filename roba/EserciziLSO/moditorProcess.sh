#! /bin/bash

if [ ! -d $1 ]
then
	echo "passa una dir"
	exit 1
fi

path=$1

pru=$(ps -u $USER | wc -l)
echo "utente $USER processi $pru"

for use in $(cat /etc/passwd | grep -v "$USER" | grep -v "/bin/false" | sed -e "s/.*x://g" -e "s/:.*//g")
do
	n=$(ps -u $use | wc -l)
	echo "utente $use processi $n"
	if [ $n -gt $pru ]
	then	
		u=$(cat /etc/passwd | grep ":$use:" | sed "s/:x:.*$//g")
		echo "l'utente $u ha eseguito i comandi: " >> "$path/utenti.txt"		
		for cmd in $(ps -u $use -o "cmd")
		do
			echo $cmd >> "$path/utenti.txt"
		done
	fi
done
