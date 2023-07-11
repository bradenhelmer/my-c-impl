CXX = clang++
CXXFLAGS = -g -std=c++14
CXXFLAGS += -Iinclude

SRC_DIR = src
INCLUDE_DIR = include
BIN = bin
OBJS_DIR = $(BIN)/objs

SRC = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(SRC))

.PHONY: all clean

all: fmt clean dirs lang

dirs:
	mkdir -p ./$(BIN) ./$(OBJS_DIR)

run: all
	$(BIN)/lang $(BIN)/"TestFile.lang"

lang: $(OBJ)
	$(CXX) -o $(BIN)/lang $^ 

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

fmt:
	clang-format -i  $(SRC) $(HEADERS)

clean:
	rm -rf $(BIN)/lang $(OBJ)
