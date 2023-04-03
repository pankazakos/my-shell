CXX = g++
FLAGS = -Wall -Wextra -g3
BIN = ./bin
BUILD = ./build
SRC = ./src
INCLUDE = ./include

all:
	$(CXX) $(SRC)/mysh.cpp -o $(BIN)/mysh $(FLAGS)

run: all
	$(BIN)/mysh

clean:
	rm -f $(BUILD)/*.o $(BIN)/mysh
