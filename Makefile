CC=clang++
CFLAGS=-Wall
LDFLAGS=

SRC_DIR=src
VPATH=$(SRC_DIR)
SOURCES=functions.cpp exception.cpp node.cpp
OBJ_DIR=objects
OBJECTS=$(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o))
HEADERS=$(SOURCES:.cpp=.h)
DEPENDS=$(addprefix $(OBJ_DIR)/, $(HEADERS:.h=.d))

LIB_DIR=lib
LIB=libEasyXml


EX_DIR=examples
EX_SOURCE=$(EX_DIR)/example1.cpp
EX_OBJ=$(EX_SOURCE:.cpp=.o)
EX_OUT=$(EX_SOURCE:.cpp=.out)

all: ex1

ex1: $(EX_OUT)

$(EX_OUT): $(LIB_DIR)/$(LIB).a $(EX_SOURCE)
	$(CC) $(CFLAGS) $(EX_SOURCE) -o $@ -L$(LIB_DIR) -l$(LIB:lib%=%)

run r:
	@cd examples; ./example1.out

lib l: $(LIB_DIR)/$(LIB).a

objects/%.o: %.cpp | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) -MMD $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(LIB_DIR)/$(LIB).a: $(SOURCES) $(OBJECTS)
	mkdir -p $(LIB_DIR)
	ar rs $(LIB_DIR)/$(LIB).a $(OBJECTS)
# recompile lib if header files change
-include $(DEPENDS)

install: lib
	mkdir -p /usr/local/include/easyXml/
	cp -fR $(addprefix $(SRC_DIR)/, $(HEADERS) namespace.h) /usr/local/include/easyXml/
	cp -f $(SRC_DIR)/easyXml.h /usr/local/include/
	cp -f $(LIB_DIR)/$(LIB).a /usr/local/lib/

uninstall:
	rm -fR /usr/local/include/easyXml
	rm -f /usr/local/include/easyXml.h
	rm -f /usr/local/$(LIB).a

clean c:
	rm -f $(EX_OUT)
	rm -Rf $(OBJ_DIR)/ $(LIB_DIR)/
