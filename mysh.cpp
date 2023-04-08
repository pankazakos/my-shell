#include "parser.hpp"
#include <cstdlib>
#include <iostream>
#include <list>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

// @author Panagiotis Kazakos sdi1900067
// My custom shell

int main() {

  std::list<std::string> history;

  while (true) {
    // Per line input logic
    // Prompt
    std::cout << "in-mysh-now:> ";

    // Input
    std::string input;
    std::getline(std::cin, input);

    // Parse
    Parser parser(input);

    const std::vector<std::string> tokens = parser.getTokens();

    const int num_tokens = parser.getNumTokens();
    // new line handle
    if (num_tokens == 0) {
      continue;
    }
    // exit handle
    if (tokens[0] == "exit") {
      return 0;
    }
    // history handle
    parser.history(history);

    // Execute commands
    pid_t pid = fork();
    if (pid < 0) {
      std::cerr << "fork failed: " << std::endl;
      return 1;
    } else if (pid == 0) {
      // child
      const char *command = tokens[0].c_str();
      execlp(command, command, nullptr);
      std::cerr << command << " is not a command" << std::endl;
      return 1;
    } else {
      // parent
      int status;
      wait(&status);
    }
  }

  return 0;
}