#!/bin/bash

if [ ! -e fileDir ]
then touch fileDir.txt
fi
for file in $( ls | grep "\<[[:upper:]]")
do
if [ -d $file ]
then echo $file >> fileDir.txt
fi
done
