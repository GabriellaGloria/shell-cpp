#include <iostream>
#include <string>

enum commands
{
  type,
  echo,
  cd,
  exit_0, // can't use exit since its defined in stdlib
  invalid
};

commands string_to_command(std::string str)
{
  if(str.find("type")!= std::string::npos) return type;
  if(str.find("echo") != std::string::npos) return echo;
  if(str.find("cd") != std::string::npos) return cd;
  if(str.find("exit") != std::string::npos) return exit_0;
  
  return invalid;
}


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
    commands current = string_to_command(input);
    if(current == echo){
      std::cout << trim(input, 5, input.length() - 1) << std::endl;
    } else if(current == type){
      commands val = string_to_command(trim(input, 5, input.length() - 1));
      if(val != invalid){
        std::cout << trim(input, 5, input.length() - 1) << " is a shell builtin" << std::endl;
      } else {
        std::cout << trim(input, 5, input.length() - 1) << ": not found" << std::endl;
      }
    } else if(current == exit_0){
      return 0;
    } else {
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
