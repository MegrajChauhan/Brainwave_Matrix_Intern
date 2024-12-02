#ifndef _IO_
#define _IO_

#include "console.hpp"

namespace atm
{
    static std::unordered_map<std::string, std::string> all_commands;
    static std::vector<std::string> history;

    void print(std::string msg);

    void println(std::string msg);

    std::string input();

    void add_command(std::string command_string, std::string command_desc);
};

#endif