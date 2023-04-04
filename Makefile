CXX = g++
FLAGS = -Wall -Wextra -g3
NAME = mysh


all:
	$(CXX) $(NAME).cpp -o $(NAME) $(FLAGS)

run: all
	./$(NAME)

clean:
	rm -f *.o $(NAME)

valgrind: all
	valgrind $(NAME)