CFLAG = -Wall -Werror -ansi -pedantic

COMPILER = g++

EXEC = rshell

SOURCES = $(wildcard src/*.cpp)


OBJECTS = $(SOURCES:.cpp=.o)

BIN = mkdir -p bin src/rshell.cpp bin/rshell

$(EXEC): $(OBJECTS)

	$(COMPILER) $(OBJECTS) -o $(EXEC)

	$(BIN)



%.o: %.cpp

	$(COMPILER) -c $(CFLAGS) $< -o $@



clean:

	rm -f $(EXEC) $(OBJECTS)

	rm -rf bin