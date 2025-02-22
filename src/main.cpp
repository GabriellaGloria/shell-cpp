#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fstream>
#include <unordered_set>
#include <set>

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

	for (char i : input)
	{
		if (i == delimiter)
			delimiter_count += 1;
	}

	if (delimiter_count < 2)
		std::cout << trim(input, 5, input.length() - 1) << std::endl;
	else
	{

		int start = -1, end = -1;
		input = trim(input, 5, input.length() - 1);
		for (int i = 0; i < input.size(); i++)
		{
			if (input[i] == delimiter && start == -1)
			{
				start = i;
			}
			else if (input[i] == delimiter && end == -1)
			{
				end = i;
			}
			if (start != -1 && end != -1)
			{
				std::string cur = "";
				for (int j = start + 1; j < end; j++)
				{
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

int do_command(std::string input)
{
	int ret = system(input.c_str());
	return ret;
}

void do_cd(std::string input)
{
	input = input.substr(input.find(" ") + 1);
	int res = chdir(input == "~" ? std::getenv("HOME") : input.c_str());
	if (res < 0)
	{
		std::cout << "cd: " << input.c_str() << ": No such file or directory" << std::endl;
	}
}

void enableRawMode()
{
	termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void disableRawMode()
{
	termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= (ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

bool get_external_command(std::string &input)
{
	std::string path = std::getenv("PATH");
	std::istringstream stream(path);

	while (!stream.eof())
	{
		std::getline(stream, path, ':');
		std::string abs_path = path;
		for (const auto &dirEntry : std::filesystem::directory_iterator(abs_path))
		{
			if (dirEntry.path().filename().string().find(input) != std::string::npos)
			{
				std::string ans = dirEntry.path().filename().string();
				ans += " ";
				std::string ret = "";
				int tmp = 0;

				while (tmp < input.length() && ans[tmp] == input[tmp])
				{
					tmp++;
				}
				for (int i = tmp; i < ans.length(); i++)
				{
					ret += ans[i];
				}
				std::cout << ret;
				std::cout << std::endl;
				return true;
			}
		}
	}
	return false;
}

std::pair<std::string, std::string> autocomplete(std::string &input)
{
	std::string path = std::getenv("PATH");
	std::istringstream stream(path);
	std::set<std::string> res;
	std::string longest_common = "";

	while (!stream.eof())
	{
		std::getline(stream, path, ':');
		std::string abs_path = path;
		for (const auto &dirEntry : std::filesystem::directory_iterator(abs_path))
		{
			if (dirEntry.path().filename().string().find(input) != std::string::npos)
			{
				std::string ans = dirEntry.path().filename().string();
				std::string ret = "";
				int tmp = 0;

				while (tmp < input.length() && ans[tmp] == input[tmp])
				{
					tmp++;
				}
				for (int i = tmp; i < ans.length(); i++)
				{
					ret += ans[i];
				}
				if (res.size() == 0)
				{
					longest_common = ret;
				}
				else
				{
					std::string tmp = "";
					for (int i = 0; i < longest_common.length(); i++)
					{
						if (longest_common[i] == ret[i])
						{
							tmp += longest_common[i];
						}
						else
						{
							break;
						}
					}
					longest_common = tmp;
				}
				res.insert(ret);
			}
		}
	}

	if (res.size() > 1)
	{
		std::cout << "\a";
	}

	std::string str = "";
	for (auto it : res)
	{
		if (str != "")
		{
			str += "  ";
		}
		str += input;
		str += it;
	}

	return std::make_pair(longest_common, str);
}

bool handleTabPress(std::string &input)
{
	if (input == "ech")
	{
		input = "echo ";
		std::cout << "o ";
		return true;
	}
	else if (input == "exi")
	{
		input = "exit ";
		std::cout << "t ";
		return true;
	}

	return false;
}

void handleDoubleTab(const std::string &input)
{
	std::string path = std::getenv("PATH");
	std::istringstream stream(path);
	std::set<std::string> ret_string;

	while (!stream.eof())
	{
		std::getline(stream, path, ':');
		for (const auto &dirEntry : std::filesystem::directory_iterator(path))
		{
			std::string filename = dirEntry.path().filename().string();
			if (filename.find(input) == 0)
			{
				ret_string.insert(filename);
			}
		}
	}

	std::cout << "\r$ " << input << "    \n";

	for (const std::string &match : ret_string)
	{
		std::cout << match << "  ";
	}
	std::cout << std::endl;

	std::cout << "$ " << input;
	std::cout.flush();
}

void readInputWithTabSupport(std::string &input)
{
	enableRawMode();
	char c;
	int tab_count = 0;
	std::string originalInput;

	while (true)
	{
		c = getchar();
		if (c == '\n')
		{
			std::cout << std::endl;
			break;
		}
		else if (c == '\t')
		{
			tab_count += 1;
			std::pair<std::string, std::string> suggestion = autocomplete(input);
			if (handleTabPress(input))
				continue;
			if (!suggestion.first.empty())
			{
				std::cout << suggestion.first;
				if (suggestion.second.find(" ") == std::string::npos)
				{
					std::cout << " ";
				}
				input += suggestion.first;
			}
			else if (!suggestion.second.empty() && tab_count == 2)
			{
				std::cout << std::endl
						  << suggestion.second << "\n$ " << input;
			}
			else
			{
				std::cout << "\a";
			}
		}
		else if (c == 127)
		{
			if (!input.empty())
			{
				input.pop_back();
				std::cout << "\b \b";
			}
		}
		else
		{
			tab_count = 0;
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

		if (command_path.empty() && (input[0] == '\'' || input[0] == '\"'))
		{
			char delimiter = input[0];
			command = "";
			bool valid = false;
			for (int i = 1; i < input.length(); i++)
			{
				if (input[i] == delimiter)
				{
					valid = true;
					break;
				}
				command += input[i];
			}
			if (valid)
			{
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
		else if (current == pwd)
		{
			do_command("pwd");
		}
		else if (current == cd)
		{
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
