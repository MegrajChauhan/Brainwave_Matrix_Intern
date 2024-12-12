#ifndef _IO_
#define _IO_

#include "console.hpp"

namespace bank
{
    static std::vector<std::string> history;
    static size_t place_in_history = 0;
    static bool secret_entered = false;
    static std::string current_input, current_input_temp;

    void print(std::string msg);

    void println(std::string msg);

    std::string input();

    void reverse_secret();

    void handle_escape_sequence();

    void up_arrow();

    void down_arrow();

    void left_arrow();

    void right_arrow();

    void show_history();

    void wait();

    std::string input_no_show();

    char getc();
};

#endif