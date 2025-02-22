#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <stdlib.h>
#include <unistd.h> 
#include <termios.h>
#include <fstream>

enum validCommands
{
	type,
	echo,
	cd,
	exit_0, // can't use exit since its defined in stdlib
	invalid,
	pwd,
	cat_quote
};

validCommands string_to_command(std::string command)
{
	command = command.substr(0, command.find(" "));

	if (command == "echo")
		return validCommands::echo;
	if (command == "cd")
		return validCommands::cd;
	if (command == "exit")
		return validCommands::exit_0;
	if (command == "type")
		return validCommands::type;
	if (command == "pwd")
		return validCommands::pwd;
	if (command == "'cat'")
		return validCommands::cat_quote;

	return invalid;
}

std::string get_path(std::string command)
{
	std::string path_env = std::getenv("PATH");
	std::stringstream stream(path_env);
	std::string path;

	while (!stream.eof())
	{
		std::getline(stream, path, ':');
		std::string abs_path = path + '/' + command;
		if (std::filesystem::exists(abs_path))
		{
			return abs_path;
		}
	}

	return "";
}

std::string trim(std::string str, int start, int ends)
{
	if ((ends - start + 1) <= 0 || (ends - start + 1) > str.length())
	{
		return "";
	}
	return str.substr(start, ends - start + 1);
}

void to_echo(std::string input)
{
	char delimiter = '\'';
	int delimiter_count = 0;

	for(char i : input){
		if(i == delimiter) delimiter_count += 1;
	}

	if(delimiter_count < 2) std::cout << trim(input, 5, input.length() - 1) << std::endl;
	else {

		int start = -1, end = -1;
		input = trim(input, 5, input.length() - 1);
		for(int i=0; i<input.size(); i++){
			if(input[i] == delimiter && start == -1){
				start = i;
			} else if(input[i] == delimiter && end == -1){
				end = i;
			}
			if(start != -1 && end != -1){
				std::string cur = "";
				for(int j=start+1; j<end; j++){
					cur += input[j];
				}
				std::cout << cur << " ";
				start = end = -1;
			}
		}
		std::cout << std::endl;
	}

}

void to_type(std::string input)
{
	validCommands val = string_to_command(trim(input, 5, input.length() - 1));
	if (val != invalid)
	{
		std::cout << trim(input, 5, input.length() - 1) << " is a shell builtin" << std::endl;
	}
	else
	{
		input = trim(input, 5, input.length() - 1);
		std::string path = get_path(input);
		if (path.empty())
		{
			std::cout << input << ": not found" << std::endl;
		}
		else
		{
			std::cout << input << " is " << path << std::endl;
		}
	}
}

void not_found(std::string input)
{
	std::cout << input << ": command not found" << std::endl;
}

void print_arguments(int counter, std::string input)
{
	if (counter == 0)
	{
		std::cout << "Arg #" << counter << " (program name): " << input << std::endl;
	}
	else
	{
		std::cout << "Arg #" << counter << ": " << input << std::endl;
	}
}

void print_signature()
{
	std::cout << "Program Signature: " << 3721638121 << std::endl;
}

int do_command(std::string input)
{
	int ret = system(input.c_str());
	return ret;
}

void do_cd(std::string input){
	input = input.substr(input.find(" ") + 1);
	int res = chdir(input == "~" ? std::getenv("HOME") : input.c_str());
	if(res < 0){
		std::cout << "cd: " << input.c_str() << ": No such file or directory" << std::endl;
	}
}

void enableRawMode() {
  termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void disableRawMode() {
  termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag |= (ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

bool handleTabPress(std::string& input) {
  if(input == "ech") {
    input = "echo ";
    std::cout << "o ";
	return true;
  }
  else if(input == "exi") {
    input = "exit ";
    std::cout << "t ";
	return true;
  }
  return false;
}

void readInputWithTabSupport(std::string& input) {
  enableRawMode();
  char c;
  while (true) {
    c = getchar();
    if (c == '\n') {
      std::cout << std::endl;
      break;
    } else if (c == '\t') {
      if(!handleTabPress(input)){
		std::cout << "\a";
	  } 
    } else if (c == 127) {
      if (!input.empty()) {
        input.pop_back();
        std::cout << "\b \b";
      }
    } else {
      input += c;
      std::cout << c;
    }
  }
  disableRawMode();
}


int main()
{
	// Flush after every std::cout / std:cerr
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	while (true)
	{
		// REPL
		std::cout << "$ ";
		std::string input;
    	readInputWithTabSupport(input);
		validCommands current = string_to_command(input);

		std::string command = input.substr(0, input.find(" "));
		std::string command_path = get_path(command);

		// std::cout << "echo " << std::endl;

		if(command_path.empty() && (input[0] == '\'' || input[0] == '\"')){
			// try remove the quote to get renamed executable
			char delimiter = input[0];
			command = "";
			bool valid = false;
			for(int i=1; i<input.length(); i++){
				if(input[i] == delimiter){
					valid = true;
					break;
				}
				command += input[i];
			}
			if(valid){
				command_path = get_path(command);
			}
		}
		if (current == type)
		{
			to_type(input);
		}
		else if (current == exit_0)
		{
			return 0;
		}
		else if(current == pwd)
		{
			do_command("pwd");
		}
		else if(current == cd)
		{
			// do_command(input);
			do_cd(input);
		}
		else if (!command_path.empty() || current == cat_quote)
		{
			do_command(input);
		} 
		else 
		{
			not_found(input);
		}
	}
}
