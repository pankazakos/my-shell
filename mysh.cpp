#include "parser.hpp"
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <list>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

// @author Panagiotis Kazakos sdi1900067
// My custom shell

static int ignore_sig = 1;
static pid_t child_pid = -1;

void signal_handler(int signal) {
  if (ignore_sig) {
    std::cout << "\nin-mysh-now:> ";
    fflush(stdout);
  } else {
    kill(child_pid, signal);
    std::cout << std::endl;
  }
}

int main() {
  std::list<std::string> history;

  // register signal handler with sigaction
  struct sigaction sa;
  sa.sa_handler = &signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  if (sigaction(SIGINT, &sa, NULL) == -1) {
    std::cerr << "signal handler not registered" << std::endl;
    return 1;
  }

  if (sigaction(SIGTSTP, &sa, NULL) == -1) {
    std::cerr << "signal handler not registered" << std::endl;
    return 1;
  }

  while (true) {
    // Per line logic
    // Prompt
    std::cout << "in-mysh-now:> ";
    fflush(stdout);

    // Input
    std::string input;
    std::getline(std::cin, input);

    // Parse
    Parser parser(input);

    const Command *tokens = parser.getTokens();

    const int num_tokens = parser.getNumTokens();

    // new line handle
    if (num_tokens == 0) {
      continue;
    }

    // add line to history
    if (history.size() == 20) {
      history.pop_front();
    }
    history.push_back(input);

    // handle keywords for all commands
    for (int i = 0; i < MAX_COMMANDS; i++) {
      const Command *command = &tokens[i]; // copy by reference
      if (command->empty)
        continue;
      // exit handle
      if (command->exec == "exit") {
        return 0;
      }
      // history handle
      parser.history(history, i);
    }

    for (int i = 0; i < MAX_COMMANDS; i++) {
      if (!tokens[i].empty) {
        std::cout << "exec: " << tokens[i].exec << std::endl
                  << "fileIn: " << tokens[i].fileIn << std::endl
                  << "fileOut: " << tokens[i].fileOut << std::endl;
        std::cout << "args: ";
        for (std::size_t j = 0; j < tokens[i].args->size(); j++) {
          std::cout << tokens[i].args->at(j) << " ";
        }
        std::cout << std::endl;
      }
    }

    // Execute commands
    for (int i = 0; i < MAX_COMMANDS; i++) {
      const Command *command = &tokens[i]; // copy by reference
      if (command->empty)
        continue;
      pid_t pid = fork();
      ignore_sig = 0;
      child_pid = pid;
      if (pid < 0) {
        std::cerr << "fork failed: " << std::endl;
        return 1;
      } else if (pid == 0) {
        // child

        // command
        const char *exec_name = command->exec.c_str();
        // arguments
        std::cout << "size: " << command->args->size() << std::endl;
        std::vector<char *> argv;
        for (auto &str : *command->args) {
          argv.push_back(&str[0]);
        }
        argv.push_back(nullptr);

        // i/o redirection
        int fdInput = open(command->fileIn.c_str(), O_RDONLY);
        int fdOutput =
            open(command->fileOut.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0644);
        dup2(fdInput, 0);
        dup2(fdOutput, 1);
        execvp(exec_name, argv.data());
        std::cerr << exec_name << " is not a command" << std::endl;
        return 1;
      } else {
        // parent

        int status;
        waitpid(pid, &status, WUNTRACED);
        ignore_sig = 1;
      }
    }
  }

  return 0;
}