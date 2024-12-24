#include "IO.hpp"

void atm::print(std::string msg)
{
    console()->append_to_body_last_line(msg);
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

std::string atm::input_no_show()
{
    Console *c = console();
    current_input.erase();
    fflush(stdin);
    bool terminate = false;
    while (!terminate)
    {
        char current = getc();
        switch (current)
        {
#ifdef _WIN32
        case '\b':
#else
        case 127:
#endif
            if (!current_input.empty())
                current_input.pop_back();
            break;
#ifdef _WIN32
        case 224:
#else
        case 27:
#endif
        {
            handle_escape_sequence();
            break;
        }
        default:
            current_input += current;
            terminate = true;
            break;
        }
        c->render();
    }
    return current_input;
}

std::string atm::input()
{
    Console *c = console();
    c->flush_first_line(); // clear the old input first
    current_input.erase();
    c->print_to_first_line(">>> ");
    fflush(stdin);
    c->render();

    // DON"T CARE ABOUT DRY PRINCIPLE
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
            if (!secret_entered)
                history.push_back(current_input);
            place_in_history = history.size();
            break;
#ifdef _WIN32
        case '\b':
#else
        case 127: // DEL
#endif
            if (!current_input.empty())
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
            else
                c->print_to_first_line('*');
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

void atm::up_arrow()
{
    if (history.empty() || place_in_history == 0)
        return;

    Console *c = console();

    if (place_in_history == history.size())
        current_input_temp = current_input;

    place_in_history--;
    c->overwrite_first_line(">>> " + history[place_in_history]);
    current_input = history[place_in_history];
}

void atm::down_arrow()
{
    if (history.empty() || place_in_history >= history.size())
        return;

    Console *c = console();

    place_in_history++;

    if (place_in_history == history.size())
    {
        c->overwrite_first_line(">>> " + current_input_temp);
        current_input = current_input_temp;
    }
    else
    {
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
    case 75:
        left_arrow();
        break;
    case 77:
        right_arrow();
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
        case 'D':
            left_arrow();
            break;
        case 'C':
            right_arrow();
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
    for (auto line : history)
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

void atm::left_arrow()
{
    // scroll up
    console()->viewport_shift(true, 1);
}

void atm::right_arrow()
{
    // scroll down
    console()->viewport_shift(false, 1);
}