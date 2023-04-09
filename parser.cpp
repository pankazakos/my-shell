#include "parser.hpp"
#include <iostream>
#include <list>
#include <vector>

Parser::Parser(std::string &str) : str(str) {
  this->split(str, ' ', this->tokens);
  for (std::size_t i = 0; i < this->tokens.size(); i++) {
    std::cout << "token: " << this->tokens[i] << std::endl;
  }
  this->num_tokens = this->tokens.size();
}

Parser::~Parser() {}

const int &Parser::getNumTokens() const { return this->num_tokens; }

const std::vector<std::string> &Parser::getTokens() const {
  return this->tokens;
}

void Parser::history(std::list<std::string> &history) {

  if (history.size() == 20) {
    history.pop_front();
  }
  history.push_back(this->str);

  if (this->tokens[0] == "h") {
    int counter = 0;
    for (const auto &line : history) {
      std::cout << counter + 1 << ") " << line << std::endl;
      counter++;
    }
    return;
  }

  int tok_length = this->tokens[0].length();
  if (tok_length == 3 || tok_length == 4) {
    if (this->tokens[0][0] == 'h' && this->tokens[0][1] == '-') {
      int index = 0;
      if (tok_length == 3) {
        index = (this->tokens[0][2] - '0') - 1;
      } else {
        std::string str_number = "";
        str_number += this->tokens[0][2];
        str_number += this->tokens[0][3];
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
      }
    }
  }
}

void Parser::split(const std::string &str, char delimiter,
                   std::vector<std::string> &substrings) {

  if (!substrings.empty()) {
    std::cout << "vector of substrings must be empty" << std::endl;
  }
  std::string curr_substr = "";
  for (std::size_t i = 0; i < str.length(); i++) {
    char ch = str[i];
    if (ch != delimiter) {
      curr_substr += ch;
    }
    if (ch == delimiter || i == str.length() - 1) {
      substrings.push_back(curr_substr);
      curr_substr = "";
    }
  }
}