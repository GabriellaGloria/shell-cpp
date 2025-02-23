#include "repl.h"
#include "command_handler.h"
#include "utils.h"
#include "autocomplete.h"
#include "terminal.h"
#include <iostream>
#include <unistd.h>
#include <string>
#include <termios.h>

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

void print_signature()
{
    std::cout << "Program Signature: " << 3721638121 << std::endl;
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