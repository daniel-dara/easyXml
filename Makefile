UNAME := $(shell uname)

# ifeq ($(UNAME), Linux)
	# CC=g++
# else ifeq ($(UNAME), Darwin)
	CC=clang++
# endif

CFLAGS=-Wall -Weffc++ -Wextra -pedantic -g
LDFLAGS=

SRC_DIR=src
# VPATH=$(SRC_DIR)
RAW_SOURCES=functions.cpp Exception.cpp Node.cpp String.cpp Input.cpp Attribute.cpp
SOURCES=$(addprefix $(SRC_DIR)/, $(RAW_SOURCES))
OBJ_DIR=obj
OBJECTS=$(addprefix $(OBJ_DIR)/, $(RAW_SOURCES:.cpp=.o))
HEADERS=$(SOURCES:.cpp=.h)
DEPENDS=$(addprefix $(OBJ_DIR)/, $(HEADERS:.h=.d))

LIB_DIR=lib
LIB=libEasyXml

EX_DIR=examples
EX_NAME=complete_example
EX_SOURCE=$(EX_DIR)/$(EX_NAME).cpp
EX_OBJ=$(EX_SOURCE:.cpp=.o)
EX_OUT=$(EX_SOURCE:.cpp=.out)

EASY_EX=easy_example

TEST_DIR=tests/exceptions
TEST_SOURCE=unit_test.cpp
TEST_OUT_DIR=$(TEST_DIR)
TEST_OUT=$(TEST_SOURCE:.cpp=.out)
TEST_CASES=test_cases.txt

FTEST_DIR=tests/functional
FTEST_OBJ_DIR=$(FTEST_DIR)/obj

FTEST_OUT=$(FTEST_DIR)/test.out
FTEST_SRCS=$(wildcard $(FTEST_DIR)/*.cpp)
FTEST_OBJS=$(addprefix $(FTEST_OBJ_DIR)/, $(notdir $(FTEST_SRCS:.cpp=.o)))
FTEST_HEADERS=$(wildcard $(FTEST_DIR)/*.h)
FTEST_DEPENDS=$(FTEST_HEADERS:.h=.d)

COMPILE_OBJECT_FILES=$(CC) -c $(CFLAGS) -MMD $< -o $@

.PHONY: info
info:
	@echo $(FTEST_OBJ_DIR)

.PHONY: all
all: ex1

.PHONY: ex1
ex1: $(EX_OUT)

$(EX_OUT): %.out: %.cpp $(LIB_DIR)/$(LIB).a
	$(CC) $(CFLAGS) $*.cpp -o $@ -L$(LIB_DIR) -l$(LIB:lib%=%)

.PHONY: ftest
ftest: $(FTEST_OUT)
	./$(FTEST_OUT)

$(FTEST_OUT): %.out: $(LIB_DIR)/$(LIB).a $(FTEST_OBJS)
	$(CC) $(CFLAGS) -MMD $(filter-out *.a,$^) -o $@ -L$(LIB_DIR) -l$(LIB:lib%=%)

$(FTEST_OBJ_DIR)/%.o: $(FTEST_DIR)/%.cpp | $(FTEST_OBJ_DIR)
	$(COMPILE_OBJECT_FILES)

$(FTEST_OBJ_DIR):
	mkdir $(FTEST_OBJ_DIR)

# recompile lib if header files change
-include $(FTEST_DEPENDS)

.PHONY: run r
run r:
	@cd examples; \
	./$(EX_NAME).out

.PHONY: simple
simple:
	cd examples; \
	g++ easy_example.cpp -lEasyXml; \
	./a.out; \
	rm -f a.out

.PHONY: lib l
lib l: $(LIB_DIR)/$(LIB).a

$(LIB_DIR)/$(LIB).a: $(SOURCES) $(OBJECTS)
	mkdir -p $(LIB_DIR)
	ar rs $(LIB_DIR)/$(LIB).a $(OBJECTS)
# recompile lib if header files change
-include $(DEPENDS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(COMPILE_OBJECT_FILES)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

.PHONY: test
test: $(TEST_DIR)/$(TEST_OUT)
	@echo "\nRunning tests..."; \
	cd $(TEST_DIR); \
	cat $(TEST_CASES) | while read line; do \
		./$(TEST_OUT) $$line; \
	done

$(TEST_OUT_DIR)/$(TEST_OUT): $(TEST_DIR)/$(TEST_SOURCE) $(LIB_DIR)/$(LIB).a | $(TEST_OUT_DIR)
	$(CC) $(CFLAGS) $< -o $@ -L$(LIB_DIR) -l$(LIB:lib%=%)

$(TEST_OUT_DIR):
	mkdir $(TEST_OUT_DIR)

.PHONY: install
install: lib
	mkdir -p /usr/local/include/easyXml/
	cp -fR $(addprefix $(SRC_DIR)/, $(HEADERS) namespace.h) /usr/local/include/easyXml/
	cp -f $(SRC_DIR)/easyXml.h /usr/local/include/
	cp -f $(LIB_DIR)/$(LIB).a /usr/local/lib/

.PHONY: uninstall
uninstall:
	rm -fR /usr/local/include/easyXml
	rm -f /usr/local/include/easyXml.h
	rm -f /usr/local/lib/$(LIB).a

.PHONY: clean
clean: clean-all

.PHONY: clean-all
clean-all: clean-lib clean-ex clean-ftest

.PHONY: clean-lib
clean-lib:
	rm -Rf $(OBJ_DIR)/ $(LIB_DIR)/

.PHONY: clean-ex
clean-ex:
	rm -f $(EX_OUT)

.PHONY: clean-ftest
clean-ftest:
	rm -f $(FTEST_DIR)/*.o $(FTEST_DIR)/*.d $(FTEST_DIR)/*.out
	rm -rf $(FTEST_OBJ_DIR)