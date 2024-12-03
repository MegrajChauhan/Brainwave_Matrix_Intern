#ifndef _CONSOLE_
#define _CONSOLE_

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/signal.h>
#endif

/**
 * Main problem:
 * We have to make sure that the body size never exceeds a certain limit
 * otherwise it will hide the first input line.
 */

namespace atm
{
    /**
     * The Console class represents what is to be printed to the console.
     * A basic renderer but just for text and the most basic of functionalities
     */
    class Console
    {

        // Two section of output pipeline
        std::string first_line;
        std::vector<std::string> body;

    public:
        Console() = default;

        void setup_console();

        void cl_init();

        void cl_reset();

        int get_console_rows();

        void console_println(std::string msg);

        void console_print(std::string msg);

        void print_to_first_line(std::string msg);

        // remove the last character from the first line
        void remove_from_fist_line();

        void flush_first_line();

        void flush_body();

        void append_to_body_last_line(char c);

        void append_to_body_last_line(std::string msg);

        void overwrite_first_line(std::string msg);

        void print_to_first_line(char c);

        void print_to_rest(std::string msg);

        void print_to_rest(char c);

        void render();

        void flush();

        void setup_resize_handler();

        ~Console();
    };

    static Console __console;

    Console *console();

#ifdef _WIN32

#else
        void handle_resize(int rows);
#endif
};

#endif