#!/bin/bash

myhome=$HOME
str=$1
if [ ! -e file$str ]
then mkdir file$str
fi
for file in $(find $myhome -name "*.$str")
do
cp $file file$str
done

