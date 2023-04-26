#include "parser.hpp"
#include <iostream>
#include <limits.h>
#include <list>
#include <map>
#include <unistd.h>
#include <vector>

Parser::Parser(std::string &str) : str(str) {

  this->num_tokens = 0;
  this->tokens = new Command[MAX_COMMANDS];
  int command_counter = 0;

  bool next_pipeIn = false;
  this->num_commands = 0;
  this->num_pipes = 0;

  for (std::size_t i = 0; i < str.length(); i++) {

    Command *command = &this->tokens[command_counter];
    // For each command
    bool append = false; // bool if there is file to append
    std::string curr_substr = "";
    int token_counter = 0;
    char prev_delimiter = '0'; // for identifying a token from the left
    while (true) {
      char ch = str[i];
      if ((int)i > 0 && str[i - 1] == ' ' && ch == ' ') {
        if (i == str.length() - 1) {
          break;
        }
        i++;
        continue;
      }
      // Do not append any of these characters to a token
      if (ch != ' ' && ch != '<' && ch != '>' && ch != '|' && ch != ';' &&
          ch != '&' && ch != '$' && ch != '"') {
        curr_substr += ch;
      }

      // found a token before any of these characters or if it is the end of the
      // string
      if (ch == ' ' || ch == '<' || ch == '>' || i == str.length() - 1 ||
          ch == '|' || ch == ';' || ch == '&' || ch == '$') {

        if (ch == '&') {
          command->background = true;
        }
        if (token_counter == 0) {
          // ingore whitespace
          if (!curr_substr.empty()) {
            command->exec = curr_substr;
            command->args->push_back(curr_substr);
            command->empty = false;
            token_counter++;
          }
          if (next_pipeIn) {
            command->pipeIn = true;
          }
        } else if (prev_delimiter == '<') {
          command->fileIn = curr_substr;
        } else if (append) {
          if (!curr_substr.empty()) {
            command->fileApnd = curr_substr;
          }
        } else if (prev_delimiter == '>') {
          command->fileOut = curr_substr;
        } else if (prev_delimiter == '$') {
          char *env = getenv(curr_substr.c_str());
          if (env != NULL) {
            command->args->push_back(std::string(env));
          }
        } else {
          if (!curr_substr.empty()) {
            command->args->push_back(curr_substr);
          }
        }
        if (str[i - 1] != '<' && str[i - 1] != '>' && str[i - 1] != '|' &&
            str[i - 1] != ';') {
          prev_delimiter = ch;
        } else if (str[i - 1] == '>' && ch == '>') {
          append = true;
        }
        curr_substr = "";
      }
      // end of the command
      if (ch == '|' || ch == ';' || i == str.length() - 1) {
        prev_delimiter = ch;
        next_pipeIn = false;
        this->num_commands++;
        if (ch == '|') {
          next_pipeIn = true;
          this->num_pipes++;
          command->pipeOut = true;
        }
        break;
      }
      i++;
    }
    command_counter++;
    this->num_tokens += token_counter;
  }
}

Parser::~Parser() { delete[] this->tokens; }

const int &Parser::getNumTokens() const { return this->num_tokens; }

const int &Parser::getNumCommands() const { return this->num_commands; }

const int &Parser::getNumPipes() const { return this->num_pipes; }

const Command *Parser::getTokens() const { return this->tokens; }

void Parser::history(std::list<std::string> &history, int command_idx) {

  Command *command = &this->tokens[command_idx];

  std::string first_tok = command->exec;

  if (first_tok != "myHistory") {
    return; // nothing to do
  }

  // make command empty since it is a keyword
  command->empty = true;

  if (command->args->size() == 1) {
    int counter = 0;
    for (const auto &line : history) {
      std::cout << counter + 1 << ") " << line << std::endl;
      counter++;
    }
    return;
  }

  // args >= 2 (arguments after the second are ignored)

  int line_idx = atoi(command->args->at(1).c_str()) - 1;

  if (line_idx < 0) {
    std::cout << "Commands in history are listed from number 1 and above "
              << std::endl;
    return;
  }

  if (line_idx < (int)history.size()) {
    std::list<std::string>::iterator it = history.begin();
    if (history.size() == 20) {
      line_idx--; // handle pop
    }
    std::advance(it, line_idx); // access line with line_idx
    std::string line = *it;
    std::cout << line << std::endl;

    // Replace myHistory i in history with the matching commands
    history.pop_back();
    history.push_back(line);

    // Replace actual command
    Parser temp_parser(line);
    const Command *temp_tokens = temp_parser.getTokens();
    *command = temp_tokens[0]; // deep copy with custom copy constructor
  }
}

void Parser::alias(std::map<std::string, std::vector<std::string>> &aliases,
                   int command_idx) {

  Command *command = &this->tokens[command_idx];

  std::string first_tok = command->exec;

  if (first_tok != "createalias" && first_tok != "destroyalias") {
    std::vector<std::string> value = aliases[first_tok];
    if (!value.empty()) {
      // if alias exists then replace the name of alias with its value
      command->exec = value.at(0);
      // add the rest of the arguments
      for (std::size_t i = 1; i < value.size(); i++) {
        command->args->push_back(value.at(i));
      }
    }
    return;
  }

  // Keywords (createalias and destroyalias)
  // mark command as empty since it is a keyword
  command->empty = true;

  std::vector<std::string> *args = command->args;
  if (args->size() < 2)
    return;
  std::string key = args->at(1);
  if (first_tok == "createalias") {
    if (args->size() >= 3) {
      // ignore first argument "createalias"
      // insert all arguments to map except starting after the second one which
      // is used as key
      std::vector<std::string> al_args(std::next(args->begin(), 2),
                                       args->end());
      aliases[key] = al_args;
    }
  } else {
    // destroyalias keyword
    if (args->size() >= 2) {
      // ignore first argument "destorylias"
      // delete from map
      aliases.erase(key);
    }
  }
}

void Parser::cd(int command_idx) {

  Command *command = &this->tokens[command_idx];

  std::string first_tok = command->exec;

  if (first_tok != "cd") {
    return; // nothing to do
  }

  // keyword is not a command
  command->empty = true;

  // change to default HOME path if there are no arguments specified after the
  // default argument
  if (command->args->size() < 2) {
    char *home = getenv("HOME");
    chdir(home);
    return;
  }

  // get current working directory
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    std::cerr << "getcwd() error" << std::endl;
    return;
  }

  // change to new directory
  std::string dir = command->args->at(1);
  chdir(dir.c_str());
}
