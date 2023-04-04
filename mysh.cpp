#include <cstdlib>
#include <iostream>
#include <list>
#include <vector>

// @author Panagiotis Kazakos sdi1900067
// My custom shell

int main() {

  std::string *history = new std::string[20];
  int commands = 0;

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
        args.push_back(arg); // add argument to list
        arg = "";            // emtpy argument
      }
    }
    if (args.size() > 0) {
      std::string first_arg = args.front();
      if (first_arg == "exit") {
        return 0;
      } else if (first_arg == "h") {
        for (int i = 0; i < commands; i++) {
          std::cout << i + 1 << ") " << history[i] << std::endl;
        }
      }
      std::string substring = "";
      int fa_length = first_arg.size();
      for (int i = 0; i < fa_length; i++) {
        char ch = first_arg[i];

        if (substring == "h-") {
          if (fa_length - 1) {
            int index = first_arg[i] - '0';
            std::cout << history[index - 1] << std::endl;
          } else if (fa_length - 2) {
            std::string temp = "" + first_arg[i - 1] + first_arg[i];
            int index = atoi(temp.c_str());
            std::cout << history[index - 1] << std::endl;
          }
        }

        substring += ch;
      }
    }
    history[commands] = input;
    commands++;
    if (commands == 20) {
      commands = 0;
    }
  }

  return 0;
}