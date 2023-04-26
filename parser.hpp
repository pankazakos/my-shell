#ifndef _MYPARSER_
#define _MYPARSER_ 1

#include <iostream>
#include <list>
#include <map>
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
  Command(const Command &cmd)
      : exec(cmd.exec), args(new std::vector<std::string>(*cmd.args)),
        fileIn(cmd.fileIn), fileOut(cmd.fileOut), fileApnd(cmd.fileApnd),
        pipeOut(cmd.pipeOut), pipeIn(cmd.pipeIn), background(cmd.background),
        empty(cmd.empty) {}
  Command &operator=(const Command &cmd) {
    if (this != &cmd) {
      exec = cmd.exec;
      fileIn = cmd.fileIn;
      fileOut = cmd.fileOut;
      fileApnd = cmd.fileApnd;
      pipeOut = cmd.pipeOut;
      pipeIn = cmd.pipeIn;
      background = cmd.background;
      empty = cmd.empty;
      delete args;
      args = new std::vector<std::string>(*cmd.args);
    }
    return *this;
  }
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

  // Array of MAX_COMMANDS * n dimensions where n is the number of tokens of
  // each command
  const Command *getTokens() const;

  // prints history when corresponding keyword is given
  // h prints whole history
  // h-i prints line of history with number i
  void history(std::list<std::string> &, int);

  // handle createalias and destroyalias keywords
  // createalias name value : creates an alias for value with name
  // destroyalias name : destroys the alias with name
  // also replaces the command with the value of the alias if exists
  void alias(std::map<std::string, std::vector<std::string>> &, int);

  // handle cd keyword
  void cd(int);
};

#endif