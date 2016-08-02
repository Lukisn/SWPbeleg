GCC = g++-5
OPT = -std=c++11 -lstdc++fs
EXE = ./fancy.x

all:
	$(GCC) fancyFunction.cpp -o $(EXE) $(OPT)

.PHONY:
clean:
	rm -f *.x
	rm -rf ./.db/
	rm -f *.db
	rm -f *.dat
	rm -f *.eps

.PHONY:
test:
	bash test.sh

.PHONY:
docs:
	doxygen Doxyfile
