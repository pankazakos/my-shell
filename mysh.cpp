#include "parser.hpp"
#include <cstdlib>
#include <iostream>
#include <list>
#include <sys/wait.h>
#include <unistd.h>

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

    const int num_tokens = parser.getNumTokens();

    // std::cout << "number of tokens: " << num_tokens << std::endl;

    parser.generateTokens();
    const std::string *tokens = parser.getTokens();

    // for (int i = 0; i < num_tokens; i++) {
    //   std::cout << "token: " << tokens[i] << std::endl;
    // }

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
    // char h_ch = tokens[0][0];
    // if (h_ch == 'h') {
    // }
    // if (tokens[0] == "h") {
    //   for (int i = 0; i < commands; i++) {
    //     std::cout << i + 1 << ") " << history[i] << std::endl;
    //   }
    // } else {
    // }

    // Execute commands
    // pid_t pid = fork();
    // if (pid < 0) {
    //   std::cerr << "fork failed: ";
    //   return 1;
    // } else if (pid == 0) {
    //   // get command from tokens

    // } else {
    //   // parent
    //   int status;
    //   wait(&status);
    // }

    // try {
    //   pid_t pid = fork();

    //   if (pid < 0) {
    //     return 1;
    //   } else if (pid == 0) {
    //     std::cout << "child " << std::endl;
    //     execlp("ls", "ls", NULL);
    //   } else {
    //     std::cout << "parent" << std::endl;
    //     int status;
    //     wait(&status);
    //   }
    // } catch (const std::bad_alloc &ex) {
    //   std::cerr << "Memory allocation failed in child process: " <<
    //   ex.what()
    //             << std::endl;
    //   return 1;
    // }

    // // Execute commands
    // pid_t pid = fork();
    // if (pid < 0) {
    //   std::cerr << "fork failed: ";
    //   return 1;
    // } else if (pid == 0) {
    //   // get command from list
    //   // std::string command = args.front().c_str();
    //   std::string command = "mycommand";
    //   // if (command.empty()) {
    //   //   continue;
    //   // }

    //   args.pop_front();
    //   // std::string second = args.front();

    //   // get path
    //   // std::string path_env = std::getenv("PATH");
    //   // if (path_env.empty()) {
    //   //   std::cout << "cannot get the PATH environment variable" <<
    //   std::endl;
    //   //   return 1;
    //   // }

    //   // // For each token in PATH env
    //   // std::stringstream ss(path_env);
    //   // std::string path;
    //   // while (std::getline(ss, path, ':')) {
    //   //   // check if executable exists
    //   //   std::string file = path + "/" + command;
    //   //   if (access(file.c_str(), F_OK) == 0) {
    //   //     // path found
    //   //     break;
    //   //   }
    //   //   // clear path
    //   //   path = "";
    //   // }
    //   execlp(command.c_str(), command.c_str(), nullptr);
    //   std::cerr << "Failed to execute " << command << std::endl;
    //   return 1;
    // } else {
    //   int status;
    //   wait(&status);

    //   history[commands] = input;
    //   commands++;
    //   if (commands == 20) {
    //     commands = 0;
    //   }
    // }
  }

  // delete[] history;
  return 0;
}
