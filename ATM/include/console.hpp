#ifndef _CONSOLE_
#define _CONSOLE_

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace atm
{
    /**
     * The Console class represents what is to be printed to the console.
     * A basic renderer but just for text and the most basic of functionalities
     */
    class Console
    {
        std::unordered_map<std::string, std::string> all_commands;
        std::string input_symbol_to_show = ">>> "; // Generally ">>> "
        std::string current_input;
        std::vector<std::string> history;
        
        // Two section of output pipeline
        std::string first_line;
        std::string the_rest_of_the_body;

    public:
        Console() = default;

        Console(std::string inp_sym) : input_symbol_to_show(inp_sym) {}

        void read_input();

        void setup_console();

        void cl_init();

        void cl_reset();

        std::string get_input();

        void console_println(std::string msg);

        void console_print(std::string msg);

        ~Console();

    };
};

#endif