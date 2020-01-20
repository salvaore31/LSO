#!/bin/bash

for file in $(ls | grep ".$1$")
do rm -i $file
done
