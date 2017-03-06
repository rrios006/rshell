CFLAG = -Wall -Werror -ansi -pedantic

COMPILER = g++

all:
	mkdir -p ./bin
	$(COMPILER) $(CFLAG) ./src/rshell.cpp ./src/test.h ./src/execute.h -o ./bin/rshell

rshell: 
	$(COMPILER) $(CFLAG) ./src/rshell.cpp -o ./bin/rshell


