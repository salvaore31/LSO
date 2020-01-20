#!/bin/bash

if [ $# -ne 2 ]
then echo "passa 2 parametri e nu uaj nir miezz e cosc"
exit 1
fi
if [ ! -e $1 ]
then echo "OHH o file"
exit 1
fi
if [ -e $2 ] && [ -d $2 ]
then cp $1 $2
else
echo "no dir"
fi
