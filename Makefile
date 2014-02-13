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
EX_HEADERS=$(addprefix $(EX_DIR)/, $(SOURCES:.cpp=.h))

EXECUTABLE=$(EX_DIR)/example1.out
EXEC_SOURCE=$(EX_DIR)/example1.cpp
EXEC_OBJ=$(EXEC_SOURCE:.cpp=.o)

all: example1

example1: $(EX_DIR)/$(LIB_H) $(EX_DIR)/namespace.h lib $(EXEC_SOURCE) \
          $(EXEC_OBJ) $(EXECUTABLE) $(EX_HEADERS)
              

.cpp.o:
	$(CC) $(CFLAGS) -MMD $< -o $@

examples/%.h: %.h
	cp $^ $@

$(EXECUTABLE): $(EXEC_SOURCE) $(LIB).a
	$(CC) -Wall $(EXEC_SOURCE) -o $@ -L. -lEasyXml

lib: $(SOURCES) $(OBJECTS) $(LIB).a
# recompile lib if header files change
-include $(DEPENDS)

$(EXTRA_INCLUDES):
	$(CC) $(CFLAGS) -MMD $@.h

$(LIB).a: $(OBJECTS)
	ar rs $(LIB).a $(OBJECTS)

run:
	@cd examples; ./example1.out

clean:
	rm -f *.o *.out *.a *.d *.gch

