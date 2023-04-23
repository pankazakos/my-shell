CXX = g++
FLAGS = -Wall -Wextra -g3 -std=c++11
EXEC = mysh
OBJS = mysh.o parser.o

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC)

mysh.o: mysh.cpp
	$(CXX) mysh.cpp -c $(FLAGS)

parser.o: parser.cpp parser.hpp
	$(CXX) parser.cpp -c $(FLAGS)

clean:
	rm -f *.o $(EXEC)
