#include "IO.hpp"

void atm::print(std::string msg)
{
    console()->print_to_rest(msg);
}

void atm::println(std::string msg)
{
    console()->print_to_rest(msg + "\r\n");
}

char atm::getc()
{
    char current;
#ifdef _WIN32
    current = _getch();
#else
    while (read(STDIN_FILENO, &current, 1) < 0)
        ;
#endif
    return current;
}

std::string atm::input()
{
    Console *c = console();
    c->flush(); // clear the old screen first
    current_input.erase();
    c->print_to_first_line(">>> ");
    c->render();
    bool terminate = false;
    while (!terminate)
    {
        char current = getc();
        // c->print_to_rest(std::string("Read character ") + current + "\r");
        switch (current)
        {
        case '\n':
        case '\r':
            terminate = true;
            // This calls for storing it as history
            history.push_back(current_input);
            place_in_history = history.size();
            break;
#ifdef _WIN32
        case '\b':
#else
        case 127: // DEL
#endif
            if (!secret_entered)
                c->remove_from_fist_line();
            if (!current_input.empty())
                current_input.pop_back();
            break;
#ifdef _WIN32
        case 224:
#else
        case 27:
#endif
        {
            // escape sequence in Linux
            handle_escape_sequence();
            break;
        }
        default:
            current_input += current;
            if (!secret_entered)
                c->print_to_first_line(current);
            break;
        }
        c->render();
    }
    return current_input;
}

void atm::reverse_secret()
{
    secret_entered = !secret_entered;
}

void atm::add_command(std::string command_string, std::string command_desc)
{
    all_commands[command_string] = command_desc;
}

void atm::up_arrow()
{
    if (history.empty() || place_in_history == 0)
        return; // No more history to navigate upwards
    
    Console *c = console();

    // Save the current input if we are navigating history for the first time
    if (place_in_history == history.size())
        current_input_temp = current_input;

    place_in_history--; // Move up in history
    c->overwrite_first_line(">>> " + history[place_in_history]); // Display the history entry
    current_input = history[place_in_history]; // Update the current input
}

void atm::down_arrow()
{
    if (history.empty() || place_in_history >= history.size())
        return; // No more history to navigate downwards
    
    Console *c = console();

    place_in_history++; // Move down in history
    
    if (place_in_history == history.size())
    {
        // If at the end of history, restore the current input
        c->overwrite_first_line(">>> " + current_input_temp);
        current_input = current_input_temp;
    }
    else
    {
        // Otherwise, show the next history entry
        c->overwrite_first_line(">>> " + history[place_in_history]);
        current_input = history[place_in_history];
    }
}

void atm::handle_escape_sequence()
{
    // We only handle Up and Down keys.
    // Working with the Left and right keys will unnecessarily complicate things
    // for a basic ATM CLI
    char next = getc();
#ifdef _WIN32
    switch (next)
    {
    case 72:
        up_arrow();
        break;
    case 80:
        down_arrow();
        break;
    }
#else
    switch (next)
    {
    case '[':
    {
        next = getc();
        switch (next)
        {
        case 'A':
            up_arrow();
            break;
        case 'B':
            down_arrow();
            break;
        }
        break;
    }
    }
#endif
}

void atm::show_history()
{
    Console *c = console();
    c->flush();
    for (auto line: history)
    {
        c->print_to_rest(line);
    }
    c->overwrite_first_line("Press any key to continue...");
    c->render();
    wait();
}

void atm::wait()
{
    getc(); // ignore this
}