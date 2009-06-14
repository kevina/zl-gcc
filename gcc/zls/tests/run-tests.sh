#!/bin/sh

for zls in *.zls;
do
  b=`basename $zls .zls`
  c=$b.c
  zls -O -S $c
  mv $b.s $c.s
  zls -O -S $zls
  mv $b.s $zls.s
  diff -I "\.file" -u $c.s $zls.s
done
