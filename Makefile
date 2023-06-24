CXX = clang++
CXXFLAGS = -Wall -Wextra -g
CXXFLAGS += -Iinclude

SRC = src/main.cpp
OBJ = $(SRC:.cpp=.o)
BIN = bin

.PHONY: all clean

all: dirs lang

dirs:
	mkdir -p ./$(BIN)

lang: $(OBJ)
	$(CXX) -o $(BIN)/lang $^ 

%.o: %.c
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf $(BIN) $(SRC)
