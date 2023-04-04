CXX = g++
FLAGS = -Wall -Wextra -g3
NAME = mysh

all: mysh.o
	$(CXX) $(NAME).o -o $(NAME)

mysh.o: mysh.cpp
	$(CXX) $(NAME).cpp -c $(FLAGS)

run: all
	./$(NAME)

clean:
	rm -f *.o $(NAME)

valgrind: all
	valgrind $(NAME)