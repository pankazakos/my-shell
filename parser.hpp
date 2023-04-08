#ifndef _MYPARSER_
#define _MYPARSER_ 1

#include <iostream>
#include <list>
#include <vector>

class Parser {
private:
  int num_tokens;
  std::string str;
  std::vector<std::string> tokens;

public:
  Parser(std::string &);
  ~Parser();
  const int &getNumTokens() const;
  const std::vector<std::string> &getTokens() const;
  void history(std::list<std::string> &);
  static void split(const std::string &, char, std::vector<std::string> &);
};

#endif