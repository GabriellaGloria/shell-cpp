#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <string>

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

validCommands string_to_command(std::string command);
int do_command(std::string input);
void do_cd(std::string input);
void to_type(std::string input);
void to_echo(std::string input);
void not_found(std::string input);

#endif // COMMAND_HANDLER_H
