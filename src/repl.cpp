#include "repl.h"
#include "command_handler.h"
#include "utils.h"
#include "autocomplete.h"
#include "terminal.h"
#include <iostream>
#include <string>

void repl()
{
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
            return;
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
