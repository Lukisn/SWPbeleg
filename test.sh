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
