#!/bin/bash

for file in $(ls)
do
	if [ -f $file ]
	then echo $file >> fileReg.txt
	fi
done

