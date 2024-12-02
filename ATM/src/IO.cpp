#include "IO.hpp"

void atm::print(std::string msg)
{
    console()->print_to_rest(msg);
}

void atm::println(std::string msg)
{
    console()->print_to_rest(msg + "\r\n");
}

std::string atm::input()
{
    Console *c = console();
    std::string current_input;
    c->print_to_first_line(">>> ");
    c->render();
    bool terminate = false;
    while (!terminate)
    {
        char current;
#ifdef _WIN32
        current = _getch();
#else
        while (read(STDIN_FILENO, &current, 1) < 0);
#endif
        c->print_to_rest(std::string("Read character ") + current + "\r");
        switch (current)
        {
        case '\n':
        case '\r':
            terminate = true;
            // This calls for storing it as history
            history.push_back(current_input);
            break;
        default:
            current_input += current;
            c->print_to_first_line(current);
            break;
        }
        c->render();
    }
    return current_input;
}

void atm::add_command(std::string command_string, std::string command_desc)
{
    all_commands[command_string] = command_desc;
}