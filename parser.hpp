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
  std::string fileApnd;           // append to file
  bool pipeOut;                   // redirect stdout to pipe write
  bool pipeIn;                    // redirect stdin to pipe read
  bool background;                // command should run in background
  bool empty; // false if command exists and is not a keyword
  Command() : pipeOut(false), pipeIn(false), background(false), empty(true) {
    this->args = new std::vector<std::string>;
  };
  ~Command() { delete args; };
};

class Parser {
private:
  int num_tokens;   // number of total tokens
  int num_commands; // number of commands
  int num_pipes;    // number of pipes
  std::string str;  // whole input from the shell prompt
  Command *tokens;  // tokens of all commands in one line of the shell prompt

public:
  // Parses mysh input
  Parser(std::string &);
  ~Parser();

  const int &getNumTokens() const;
  const int &getNumCommands() const;
  const int &getNumPipes() const;

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