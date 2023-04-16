CXX = g++
FLAGS = -Wall -Wextra -g3 -std=c++11
NAME = mysh
OBJS = mysh.o parser.o

all: mysh.o parser.o
	$(CXX) $(OBJS) -o $(NAME)

mysh.o: mysh.cpp
	$(CXX) $(NAME).cpp -c $(FLAGS)

parser.o: parser.cpp parser.hpp
	$(CXX) parser.cpp -c $(FLAGS)

clean:
	rm -f *.o $(NAME)
