#include <cstdlib>
#include <iostream>
#include <list>

// @author Panagiotis Kazakos sdi1900067
// My custom shell

int main() {

  while (true) {
    // Per line input logic
    // Prompt
    std::cout << "in-mysh-now:> ";

    // Input
    std::string input;
    std::getline(std::cin, input);

    // Parse
    std::list<std::string> args; // list of arguments
    std::string arg = "";        // each argument

    for (std::string::size_type i = 0; i < input.length(); i++) {
      char ch = input[i];
      if (ch != ' ') {
        arg += ch;
      }
      if (ch == ' ' || i == input.length() - 1) {
        std::cout << arg << std::endl;
        args.push_back(arg); // add argument to list
        arg = "";            // emtpy argument
      }
    }

    if (args.size() > 0 && args.front() == "exit") {
      exit(EXIT_SUCCESS);
    }
  }

  return 0;
}