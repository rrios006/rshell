CFLAG = -Wall -Werror -ansi -pedantic

COMPILER = g++

all: rshell

rshell: 
	$(COMPILER) $(CFLAG) ./src/rshell.cpp -o ./bin/rshell

