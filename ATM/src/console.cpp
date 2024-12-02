#include "console.hpp"

void atm::Console::setup_console()
{
    // prepare the console
    cl_init();
}

void atm::Console::cl_init()
{
#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    // Disable line input and echo
    SetConsoleMode(hStdin, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
#else
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);

    raw.c_lflag &= ~(ECHO | ICANON | ISIG); // Disable echo, canonical mode, and signals
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
#endif
}

void atm::Console::cl_reset()
{
#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    // Re-enable line input and echo
    SetConsoleMode(hStdin, mode | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
#else
    struct termios original;
    tcgetattr(STDIN_FILENO, &original);

    original.c_lflag |= (ECHO | ICANON | ISIG); // Re-enable echo, canonical mode, and signals
    tcsetattr(STDIN_FILENO, TCSANOW, &original);
#endif
}

void atm::Console::read_input()
{
    // we will read character by character and read until a newline
    // With each character, we may utilize some characters for special
    // commands that the Console class may execute
    console_print(input_symbol_to_show);
    bool terminate = false;
    while (!terminate)
    {
        char current;
#ifdef _WIN32
        current = _getch();
#else
        if (read(STDIN_FILENO, &current, 1) < 0)
        {
            console_println("Failed to read input.");
            cl_reset();
            exit(EXIT_FAILURE);
        }
#endif
        console_println(std::string("Read character ") + current);
        switch (current)
        {
        case '\n':
            terminate = true;
            break;
        default:
            current_input += current;
            console_print(std::string({current}));
            break;
        }
    }
    console_println("Input read: " + current_input);
}

void atm::Console::console_println(std::string msg)
{
    // specifically for writing when in raw mode
    std::cout << msg << "\r\n";
}

void atm::Console::console_print(std::string msg)
{
    // specifically for writing when in raw mode
    std::cout << msg;
}

atm::Console::~Console()
{
    cl_reset();
}

std::string atm::Console::get_input()
{
    std::string inp;
    if (current_input.empty())
    {
        read_input();
        inp = current_input;
        current_input.erase();
    }
    else
        inp = current_input;
    return inp;
}