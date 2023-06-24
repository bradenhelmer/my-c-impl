CXX = clang++
CXXFLAGS = -Wall -Wextra -g
CXXFLAGS += -Iinclude

SRC_DIR = src
BIN = bin
OBJS_DIR = $(BIN)/objs

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(SRC))

.PHONY: all clean

all: dirs lang

dirs:
	mkdir -p ./$(BIN) ./$(OBJS_DIR)

run: all
	$(BIN)/lang

lang: $(OBJ)
	$(CXX) -o $(BIN)/lang $^ 

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
