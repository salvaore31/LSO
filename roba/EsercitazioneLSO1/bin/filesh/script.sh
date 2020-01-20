#! /bin/sh
numero=1
number=01
nombre=" 1"
if test $numero -eq $number ; then echo ok1; fi
if test $numero = $number ; then echo ok2; fi
if test $numero -eq $nombre ; then echo ok3; fi
if test $numero = $nombre ; then echo ok4; fi
echo $numero
echo $nombre
