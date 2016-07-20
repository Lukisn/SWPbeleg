GCC = g++-5
OPT = -std=c++11 -lstdc++fs
EXE = ./fancy.x

all:
	$(GCC) fancyFunction.cpp -o $(EXE) $(OPT)

.PHONY:
clean:
	rm -f *.x
	rm -rf ./.db/

.PHONY:
test:
	bash test.sh
