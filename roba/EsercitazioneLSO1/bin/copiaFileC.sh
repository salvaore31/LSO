#!/bin/bash

cd $HOME
for file in $(ls -R | grep ".*\.c\>")
do
mkdir fileC
cp $file $HOME/fileC
done
