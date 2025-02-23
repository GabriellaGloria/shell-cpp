#include "repl.h"
#include "command_handler.h"
#include "utils.h"
#include "autocomplete.h"
#include "terminal.h"
#include <iostream>
#include <unistd.h>
#include <termios.h>
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