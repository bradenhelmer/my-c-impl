CXX = clang++

CXXFLAGS = -g
CXXFLAGS += -Iinclude

LLVM_LDFLAGS = $(shell llvm-config --system-libs --libs core)
LLVM_CXXFLAGS = $(shell llvm-config --cxxflags)

CXXFLAGS += $(LLVM_CXXFLAGS)
LDFLAGS ?= $(LLVM_LDFLAGS)

SRC_DIR = src
INCLUDE_DIR = include
BIN = bin
OBJS_DIR = $(BIN)/objs

SRC = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(SRC))

RUN_ARGS = $(BIN)/lang --exec $(BIN)/"TestFile.lang"
PRINT_ARGS = $(BIN)/lang --print $(BIN)/"TestFile.lang"

.PHONY: all clean

all: clean dirs lang

dbg: clean dirs lang
	gdb -q --args $(RUN_ARGS)

dirs:
	mkdir -p ./$(BIN) ./$(OBJS_DIR)

run: all
	$(RUN_ARGS)

print: all
	$(PRINT_ARGS)

re:
	$(BIN)/lang --exec $(BIN)/"TestFile.lang"

rep:
	$(BIN)/lang --print $(BIN)/"TestFile.lang"

lang: $(OBJ)
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $(BIN)/lang

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< 

fmt:
	clang-format -i  $(SRC) $(HEADERS)

clean:
	rm -rf $(BIN)/lang $(OBJ)
