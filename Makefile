CC=clang++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=functions.cpp exception.cpp node.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPENDS=$(OBJECTS:.o=.d)
HEADERS=$(SOURCES:.cpp=.h)
LIB_H=easyXml.h
LIB=libEasyXml

EX_DIR=examples
EXECUTABLE=$(EX_DIR)/example1.out
EXEC_SOURCE=$(EX_DIR)/example1.cpp
EXEC_OBJ=$(EXEC_SOURCE:.cpp=.o)

all: example1

example1: lib $(EXEC_SOURCE) $(EXECUTABLE)

$(EXECUTABLE): $(EXEC_SOURCE) $(LIB).a
	$(CC) -Wall $(EXEC_SOURCE) -o $@ -L. -lEasyXml

run r:
	@cd examples; ./example1.out


lib: $(SOURCES) $(OBJECTS) $(LIB).a
# recompile lib if header files change
-include $(DEPENDS)

.cpp.o:
	$(CC) $(CFLAGS) -MMD $< -o $@

$(EXTRA_INCLUDES):
	$(CC) $(CFLAGS) -MMD $@.h

$(LIB).a: $(OBJECTS)
	ar rs $(LIB).a $(OBJECTS)

clean c:
	rm -f *.o *.out *.a *.d *.gch

