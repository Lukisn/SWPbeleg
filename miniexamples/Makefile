GCC = g++-5

all: folders mapfile

folders:
	$(GCC) folders.cpp -o folders.x -std=c++11 -lstdc++fs

mapfile:
	$(GCC) mapfile.cpp -o mapfile.x -std=c++11

clean:
	rm -f *.x *.db
	rm -rf ./a/
