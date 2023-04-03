#include <cstdlib>
#include <iostream>

// @author Panagiotis Kazakos sdi1900067
// My custom shell

int main() {

  while (true) {
    // Print prompt
    std::cout << "in-mysh-now:> ";

    // Get input
    std::string input;
    std::getline(std::cin, input);
  }

  return 0;
}