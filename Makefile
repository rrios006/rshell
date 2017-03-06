CFLAG = -Wall -Werror -ansi -pedantic

COMPILER = g++

EXEC = rshell

SOURCES = $(wildcard *.cpp)


OBJECTS = $(SOURCES:.cpp=.o)

BIN = ./src/rshell.cpp -o ./bin/rshell

$(EXEC): $(OBJECTS)

	$(COMPILER) $(OBJECTS) -o $(EXEC)

	$(BIN)



clean:

	rm -f $(EXEC) $(OBJECTS)

	rm -rf bin