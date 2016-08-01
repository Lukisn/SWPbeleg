#!/bin/bash

EXE=./fancy.x

echo "removing database..."
rm -rf ./.db/

echo "empty database, adding 1 2 3 4..."
$EXE 1 2 3 4
echo "retrieving 1 2 3 4..."
$EXE 1 2 3 4

echo "adding 1.1 2 3.00000002 4.10002..."
$EXE 1.1 2 3.00000002 4.10002
echo "retrieving 1.1 2 3.00000002 4.10002..."
$EXE 1.1 2 3.00000002 4.10002

echo "addin 2d data..."
$EXE 1 1
$EXE 1 2
$EXE 1 3
$EXE 2 1
$EXE 2 2
$EXE 2 3
$EXE 3 1
$EXE 3 2
$EXE 3 3

echo "gnuifiing dumped data..."
python3 gnuify.py dump.db plot.dat

echo "plotting data..."
gnuplot plot.gnu
