GCC = g++-5
OPT = -std=c++11

all: folders mapfile

folders:
	$(GCC) folders.cpp -o folders.x $(OPT) -lstdc++fs

mapfile:
	$(GCC) mapfile.cpp -o mapfile.x $(OPT)

.PHONY:
clean:
	rm -f *.x *.db
	rm -rf ./a/
