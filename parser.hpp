#ifndef _MYPARSER_
#define _MYPARSER_ 1

#include <iostream>
#include <list>

class Parser {
private:
  int num_tokens;
  std::string str;
  std::string *tokens;

public:
  Parser(std::string &);
  ~Parser();
  const int &getNumTokens() const;
  const std::string *getTokens() const;
  void generateTokens();
  void history(std::list<std::string> &);
};

#endif