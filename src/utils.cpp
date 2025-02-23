#include "utils.h"
#include <sstream>
#include <iostream>
#include <filesystem>
#include <string>

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