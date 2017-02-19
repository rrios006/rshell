CFLAG = -Wall -Werror -ansi -pedantic

COMPILER = g++

all: rshell

rshell:
	mkdir -p bin
	$(COMPLIER) $(CFLAG) ./src/rshell.cpp -0 ./bin/rshell
