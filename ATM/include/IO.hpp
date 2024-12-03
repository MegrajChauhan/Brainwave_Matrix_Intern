#ifndef _IO_
#define _IO_

#include "console.hpp"

namespace atm
{
    static std::unordered_map<std::string, std::string> all_commands;
    static std::vector<std::string> history;
    static size_t place_in_history = 0;
    static bool secret_entered = false;
    static std::string current_input, current_input_temp;

    void print(std::string msg);

    void println(std::string msg);

    std::string input();

    void reverse_secret();

    void add_command(std::string command_string, std::string command_desc);

    void handle_escape_sequence();

    void up_arrow();

    void show_history();

    void wait();

    void down_arrow();

    char getc();
};

#endif