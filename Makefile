CFLAG = -Wall -Werror -ansi -pedantic

COMPILER = g++

all: rshell

rshell:
	mkdir -p bin
	$(COMPLIER) $(CFLAG) src/main.cpp -0 bin/rshell
