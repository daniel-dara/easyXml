CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=functions.cpp exception.cpp parser.cpp node.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPENDS=$(OBJECTS:.o=.d)
HEADERS=$(SOURCES:.cpp=.h)
LIB_H=easyXml.h
LIB=libEasyXml
EX_DIR=examples
EX_HEADERS=$(addprefix $(EX_DIR)/, $(SOURCES:.cpp=.h))

EXECUTABLE=$(EX_DIR)/example.out
EXEC_SOURCE=$(EX_DIR)/example.cpp
EXEC_OBJ=$(EXEC_SOURCE:.cpp=.o)

all: lib example1

example1: lib $(EXEC_SOURCE) $(EXEC_OBJ) $(EXECUTABLE) $(EX_HEADERS) \
              $(EX_DIR)/$(LIB_H)

.cpp.o:
	$(CC) $(CFLAGS) -MMD $< -o $@

examples/%.h: %.h
	cp $^ $@

$(EXECUTABLE): $(LIB).a
	$(CC) -Wall $(EXEC_SOURCE) -o $@ -L. -lEasyXml

lib: $(SOURCES) $(OBJECTS) $(LIB).a
# recompile lib if header files change
-include $(DEPENDS)

$(EXTRA_INCLUDES):
	$(CC) $(CFLAGS) -MMD $@.h

$(LIB).a: $(OBJECTS)
	ar rs $(LIB).a $(OBJECTS)

run:
	./$(EX_DIR)/$(EXECUTABLE)

clean:
	rm -f *.o *.out *.a *.d *.gch

