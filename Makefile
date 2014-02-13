CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=functions.cpp exception.cpp parser.cpp node.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=example1.out
EXEC_SOURCE=example1.cpp
EXEC_OBJ=$(EXEC_SOURCE:.cpp=.o)
LIB=libEasyXml
INCLUDES=easyXml.h

all: lib example

example: lib $(EXEC_SOURCE) $(EXEC_OBJ) $(EXECUTABLE)

$(EXECUTABLE):
	$(CC) -Wall $(EXEC_SOURCE) -o $@ -L. -lEasyXml

lib: $(SOURCES) $(OBJECTS) $(LIB).a $(INCLUDES)

$(LIB).a:
	ar rvs $(LIB).a $(OBJECTS)

objects:
	$(CC) $(CFLAGS) $(SOURCES)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

run:
	./$(EXECUTABLE)

clean:
	rm -f *.o *.out *.a

#$(EXECUTABLE): $(OBJECTS) 
	#$(CC) $(LDFLAGS) $(OBJECTS) -o $@
