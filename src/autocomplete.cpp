#include "repl.h"
#include "command_handler.h"
#include "utils.h"
#include "autocomplete.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <set>

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
        std::cout << "\a";
    std::string str = "";
    for (auto it : res)
    {
        if (str != "")
            str += "  ";
        str += input;
        str += it;
    }

    return std::make_pair(longest_common, str);
}