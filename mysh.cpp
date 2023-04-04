#include <cstdlib>
#include <iostream>
#include <list>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>

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

    // // Execute commands
    pid_t pid = fork();
    if (pid < 0) {
      std::cerr << "fork failed: ";
      return 1;
    } else if (pid == 0) {
      // get command from list
      std::string command = args.front();

      // get path
      std::string path_env = std::getenv("PATH");
      if (path_env.empty()) {
        std::cout << "cannot get the PATH environment variable" << std::endl;
        return 1;
      }

      // For each token in PATH env
      std::stringstream ss(path_env);
      std::string path;
      while (std::getline(ss, path, ':')) {
        // check if executable exists
        std::string file = path + "/" + command;
        if (access(file.c_str(), F_OK) == 0) {
          // path found
          break;
        }
        // clear path
        path = "";
      }
      std::cout << "path: " << path << std::endl;
      execlp(("/bin/" + command).c_str(), command.c_str(), nullptr);
      std::cerr << "Failed to execute " << command << std::endl;
      return 1;
    } else {
      int status;
      wait(&status);
    }

    history[commands] = input;
    commands++;
    if (commands == 20) {
      commands = 0;
    }
  }

  return 0;
}