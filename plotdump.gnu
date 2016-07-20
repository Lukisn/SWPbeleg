#!/bin/gnuplot

set terminal qt

splot "plot.dat" with points pointsize 5 pointtype 7

pause -1 "continue with [Enter]"

set terminal postscript enhanced color
set output "dump.eps"

replot

reset
exit
