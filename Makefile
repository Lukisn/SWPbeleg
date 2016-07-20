GCC = g++-5
OPT = -std=c++11

all:
	$(GCC) fancyFunction.cpp -o fancy.x $(OPT)

clean:
	rm -f *.x
