#include "parser.hpp"
#include <iostream>
#include <list>

Parser::Parser(std::string &str) : str(str) {
  int num_tokens = 0;
  if (str.length() > 0) {
    num_tokens++;
  }
  for (std::string::size_type i = 0; i < str.length(); i++) {
    char ch = str[i];
    if (ch == ' ') {
      num_tokens++;
    }
  }
  this->num_tokens = num_tokens;
  this->tokens = new std::string[num_tokens];
}

Parser::~Parser() { delete[] this->tokens; }

const int &Parser::getNumTokens() const { return this->num_tokens; }

const std::string *Parser::getTokens() const { return this->tokens; }

void Parser::generateTokens() {
  std::string token = ""; // each token

  int counter_tokens = 0;
  for (std::string::size_type i = 0; i < str.length(); i++) {
    char ch = str[i];
    if (ch != ' ') {
      token += ch;
    }
    if (ch == ' ' || i == str.length() - 1) {
      if (token.empty()) {
        continue; // ignore whitespace
      }
      this->tokens[counter_tokens] = token;
      counter_tokens++;
      token = ""; // empty token
    }
  }
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
      if (index < (int)history.size()) {
        std::list<std::string>::iterator it = history.begin();
        std::advance(it, index);
        std::cout << *it << std::endl;
      }
    }
  }
}