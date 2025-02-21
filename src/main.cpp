#include <iostream>
#include <string>

std::string trim(std::string str, int start, int ends){
    if ((ends - start + 1) <= 0 || (ends - start + 1) > str.length()) {
        return ""; 
    }
    return str.substr(start, ends - start + 1);
} 

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while(true) {
    // REPL
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);
    if(trim(input, 0, 3) == "echo"){
      std::cout << trim(input, 5, input.length() - 1) << std::endl;
    } else if(input == "exit 0"){
      return 0;
    } else {
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
