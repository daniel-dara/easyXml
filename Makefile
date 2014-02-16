CC=clang++
CFLAGS=-Wall
LDFLAGS=

SRC_DIR=src
VPATH=$(SRC_DIR)
SOURCES=functions.cpp exception.cpp node.cpp
OBJ_DIR=obj
OBJECTS=$(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o))
HEADERS=$(SOURCES:.cpp=.h)
DEPENDS=$(addprefix $(OBJ_DIR)/, $(HEADERS:.h=.d))

LIB_DIR=lib
LIB=libEasyXml

EX_DIR=examples
EX_SOURCE=$(EX_DIR)/example1.cpp
EX_OBJ=$(EX_SOURCE:.cpp=.o)
EX_OUT=$(EX_SOURCE:.cpp=.out)

TEST_DIR=tests
TEST_OBJ_DIR=$(TEST_DIR)/obj
TEST_OUT_DIR=$(TEST_DIR)/out
TEST_SOURCES=$(wildcard $(TEST_DIR)/*.cpp)
TEMP=$(TEST_SOURCES:.cpp=.o)
TEST_OBJS=$(TEMP:$(TEST_DIR)/%=$(TEST_OBJ_DIR)/%)
TEMP=$(TEST_SOURCES:.cpp=.out)
TEST_OUTS=$(TEMP:$(TEST_DIR)/%=$(TEST_OUT_DIR)/%)

ARGS=$(MAKECMDGOALS)

all: ex1

ex1: $(EX_OUT)

$(EX_OUT): $(LIB_DIR)/$(LIB).a $(EX_SOURCE)
	$(CC) $(CFLAGS) $(EX_SOURCE) -o $@ -L$(LIB_DIR) -l$(LIB:lib%=%)

run r:
	@cd examples; ./example1.out

lib l: $(LIB_DIR)/$(LIB).a

$(LIB_DIR)/$(LIB).a: $(SOURCES) $(OBJECTS)
	mkdir -p $(LIB_DIR)
	ar rs $(LIB_DIR)/$(LIB).a $(OBJECTS)
# recompile lib if header files change
-include $(DEPENDS)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) -MMD $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

test: $(TEST_OUTS)
	@cd $(TEST_OUT_DIR); \
	for file in *; do \
		./$$file; \
	done

$(TEST_OUT_DIR)/%.out: $(TEST_DIR)/%.cpp $(LIB_DIR)/$(LIB).a | $(TEST_OUT_DIR)
	$(CC) $(CFLAGS) $< -o $@ -L$(LIB_DIR) -l$(LIB:lib%=%)

$(TEST_OUT_DIR):
	mkdir $(TEST_OUT_DIR)

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | $(TEST_OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(TEST_OBJ_DIR):
	mkdir $(TEST_OBJ_DIR)

install: lib
	mkdir -p /usr/local/include/easyXml/
	cp -fR $(addprefix $(SRC_DIR)/, $(HEADERS) namespace.h) /usr/local/include/easyXml/
	cp -f $(SRC_DIR)/easyXml.h /usr/local/include/
	cp -f $(LIB_DIR)/$(LIB).a /usr/local/lib/

uninstall:
	rm -fR /usr/local/include/easyXml
	rm -f /usr/local/include/easyXml.h
	rm -f /usr/local/lib/$(LIB).a

clean: clean-all

clean-all: clean-lib clean-ex clean-test

clean-lib:
	rm -Rf $(OBJ_DIR)/ $(LIB_DIR)/

clean-ex:
	rm -f $(EX_OUT)

clean-test:
	rm -Rf $(TEST_OUT_DIR)