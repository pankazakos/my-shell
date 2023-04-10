#ifndef _MYPARSER_
#define _MYPARSER_ 1

#include <iostream>
#include <list>
#include <vector>

#define MAX_COMMANDS 4

// Command struct contains useful information about each command that was parsed
// from input of mysh
struct Command {
  std::string exec;               // name of executable (program)
  std::vector<std::string> *args; // arguments of the program
  std::string fileIn;             // input file
  std::string fileOut;            // output file
  bool empty; // false if command exists and is not a keyword
  Command() : empty(true) { this->args = new std::vector<std::string>; };
  ~Command() { delete args; };
};

class Parser {
private:
  int num_tokens;  // number of tokens
  std::string str; // whole input from the shell prompt
  Command *tokens; // tokens of all commands in one line of the shell prompt

public:
  // Parses mysh input
  Parser(std::string &);
  ~Parser();

  // returns the number of the total tokens
  const int &getNumTokens() const;

  // Tokens MAX_COMMANDS * n array where n is the number of tokens of each
  // command
  const Command *getTokens() const;

  // prints history when corresponding keyword is given
  // h prints whole history
  // h-i prints line of history with number i
  void history(std::list<std::string> &, int);

  // splits a string to a vector (initially passed as empty) of tokens with one
  // char delimiter
  static void split(const std::string &, char, std::vector<std::string> &);
};

#endif