#ifndef _MYPARSER_
#define _MYPARSER_ 1

#include <iostream>
#include <list>
#include <vector>

#define MAX_COMMANDS 4

struct Command {
  std::string exec;               // name of executable (program)
  std::vector<std::string> *args; // arguments of the program
  std::string fileIn;             // input file
  std::string fileOut;            // output file
  bool empty;                     // if false then command exists
  Command() : empty(true) { this->args = new std::vector<std::string>; };
  ~Command() { delete args; };
};

class Parser {
private:
  int num_tokens;  // number of tokens
  std::string str; // whole input from the shell prompt
  Command *tokens; // tokens of all commands in one line of the shell prompt

public:
  Parser(std::string &);
  ~Parser();
  const int &getNumTokens() const;
  const Command *getTokens() const;
  void history(std::list<std::string> &, int);
  static void split(const std::string &, char, std::vector<std::string> &);
};

#endif