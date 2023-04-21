#include "parser.hpp"
#include <iostream>
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

    // for each command
    bool append = false;
    std::string curr_substr = "";
    int token_counter = 0;
    char prev_delimiter = '0';
    while (true) {
      char ch = str[i];
      if ((int)i > 0 && str[i - 1] == ' ' && ch == ' ') {
        if (i == str.length() - 1) {
          break;
        }
        i++;
        continue;
      }
      if (ch != ' ' && ch != '<' && ch != '>' && ch != '|' && ch != ';' &&
          ch != '&' && ch != '$' && ch != '"') {
        curr_substr += ch;
      }

      if (ch == ' ' || ch == '<' || ch == '>' || i == str.length() - 1 ||
          ch == '|' || ch == ';' || ch == '&' || ch == '$') {

        if (ch == '&') {
          this->tokens[command_counter].background = true;
        }
        if (token_counter == 0) {
          // ingore whitespace
          if (!curr_substr.empty()) {
            this->tokens[command_counter].exec = curr_substr;
            this->tokens[command_counter].args->push_back(curr_substr);
            this->tokens[command_counter].empty = false;
            token_counter++;
          }
          if (next_pipeIn) {
            this->tokens[command_counter].pipeIn = true;
          }
        } else if (prev_delimiter == '<') {
          this->tokens[command_counter].fileIn = curr_substr;
        } else if (append) {
          if (!curr_substr.empty()) {
            this->tokens[command_counter].fileApnd = curr_substr;
          }
        } else if (prev_delimiter == '>') {
          this->tokens[command_counter].fileOut = curr_substr;
        } else if (prev_delimiter == '$') {
          char *env = getenv(curr_substr.c_str());
          if (env != NULL) {
            this->tokens[command_counter].args->push_back(std::string(env));
          }
        } else {
          if (!curr_substr.empty()) {
            this->tokens[command_counter].args->push_back(curr_substr);
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
          this->tokens[command_counter].pipeOut = true;
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

  std::string first_tok = this->tokens[command_idx].exec;

  if (first_tok == "h") {
    int counter = 0;
    for (const auto &line : history) {
      std::cout << counter + 1 << ") " << line << std::endl;
      counter++;
    }
    // make command empty since it is a keyword
    this->tokens[command_idx].empty = true;
    return;
  }

  int tok_length = first_tok.length();
  if (tok_length == 3 || tok_length == 4) {
    if (first_tok[0] == 'h' && first_tok[1] == '-') {
      int index = 0;
      if (tok_length == 3) {
        index = (first_tok[2] - '0') - 1;
      } else {
        std::string str_number = "";
        str_number += first_tok[2];
        str_number += first_tok[3];
        const char *ch_number = str_number.c_str();
        index = atoi(ch_number) - 1;
      }
      if (index < 0) {
        std::cout << "Commands in history are listed from number 1 and above "
                  << index << std::endl;
      }
      if (index < (int)history.size()) {
        std::list<std::string>::iterator it = history.begin();
        if (history.size() == 20) {
          index--; // handle pop
        }
        std::advance(it, index);
        std::cout << *it << std::endl;
        this->tokens[command_idx].empty = true;
      }
    }
  }
}

void Parser::alias(std::map<std::string, std::vector<std::string>> &aliases,
                   int command_idx) {

  std::string first_tok = this->tokens[command_idx].exec;

  if (first_tok != "createalias" && first_tok != "destroyalias") {
    std::vector<std::string> value = aliases[first_tok];
    if (!value.empty()) {
      // if alias exists then replace the name of alias with its value
      this->tokens[command_idx].exec = value.at(0);
      // add the rest of the arguments
      for (std::size_t i = 1; i < value.size(); i++) {
        this->tokens[command_idx].args->push_back(value.at(i));
      }
    }
    return;
  }

  // Keywords (createalias and destroyalias)
  // mark command as empty since it is a keyword
  this->tokens[command_idx].empty = true;

  std::vector<std::string> *args = this->tokens[command_idx].args;
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
