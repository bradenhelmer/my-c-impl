CXX = clang++

CXXFLAGS = -g
CXXFLAGS += -Iinclude

LLVM_LDFLAGS = $(shell llvm-config --system-libs --libs core orcjit native)
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

.PHONY: all clean obj

all: clean dirs lang

dbgr:
	gdb -q --args $(RUN_ARGS)

dbgp:
	gdb -q --args $(PRINT_ARGS)

dirs:
	mkdir -p ./$(BIN) ./$(OBJS_DIR)

run: lang
	$(RUN_ARGS)

print: all
	$(PRINT_ARGS)

re:
	$(BIN)/lang --exec $(BIN)/"TestFile.lang"

rep:
	$(BIN)/lang --print $(BIN)/"TestFile.lang"

obj: $(OBJ)

lang: obj
	$(CXX) $(OBJ) $(CXXFLAGS) $(LDFLAGS) -o $(BIN)/lang


$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< 

singler: single lang re
singlep: single lang rep

.PHONY: single
single:
	if [ -z "$(FILE)" ]; then \
			echo "Missing .cpp file specified"; \
		else \
			$(CXX) $(CXXFLAGS) -o $(OBJS_DIR)/$(notdir $(FILE)).o -c $(FILE); \
		fi

fmt:
	clang-format -i  $(SRC) $(HEADERS)

clean:
	rm -rf $(BIN)/lang $(OBJ)
