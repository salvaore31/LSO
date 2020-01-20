#!/bin/bash
i=1
while [ $i -le $1 ]
do
touch node$i.html
i=$((i+1))
done
