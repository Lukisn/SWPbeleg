GCC = g++-5
OPT = -std=c++11 -lstdc++fs

all:
	$(GCC) fancyFunction.cpp -o fancy.x $(OPT)

clean:
	rm -f *.x
