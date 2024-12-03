#include "console.hpp"

atm::Console *atm::console()
{
    return &__console;
}

void atm::Console::setup_console()
{
    // prepare the console
    cl_init();
    setup_resize_handler();
    std::cout.setf(std::ios::unitbuf); // disable buffering for output
    update_viewport();
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

void atm::Console::print_to_first_line(std::string msg)
{
    first_line += msg;
}

void atm::Console::print_to_first_line(char c)
{
    first_line += c;
}

void atm::Console::print_to_rest(std::string msg)
{
    // Each message is going to be one line long
    body.push_back(msg);
}

void atm::Console::print_to_rest(char c)
{
    body.push_back(std::string({c}));
}

void atm::Console::remove_from_fist_line()
{
    if (!first_line.empty())
        first_line.pop_back();
}

void atm::Console::flush_first_line()
{
    first_line.erase();
}

void atm::Console::overwrite_first_line(std::string msg)
{
    first_line = msg;
}

void atm::Console::render()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    for (int i = body.size(); i < viewport.ed; i++)
    {
        body.push_back("");
    }
    for (std::string s : body)
    {
        console_println(s);
    }
    console_print(first_line);
}

void atm::Console::flush()
{
    first_line.erase();
    body.erase(body.begin(), body.end());
}

int atm::Console::get_console_rows()
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        return -1;
    }
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
    {
        return -1;
    }
    return w.ws_row;
#endif
}

#ifdef _WIN32

#else
void atm::handle_resize(int rows)
#endif
{
    __console.update_viewport();
    __console.render();
}

void atm::Console::update_viewport()
{
    int row_count = __console.get_console_rows();

    viewport.ed = row_count - 1;

    // based on the size of the body
    if ((row_count - 1) > body.size())
        viewport.st = 0;
    // we don't do anything else here
}

void atm::Console::flush_body()
{
    body.erase(body.begin(), body.end());
}

void atm::Console::setup_resize_handler()
{
#ifdef _WIN32
    SetConsoleCtrlHandler(handle_resize, TRUE);
#else
    struct sigaction sa;
    sa.sa_handler = handle_resize;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGWINCH, &sa, nullptr) == -1)
    {
        std::cerr << "Failed to set resize handler!" << std::endl;
        cl_reset();
        exit(EXIT_FAILURE);
    }
#endif
}

void atm::Console::append_to_body_last_line(char c)
{
    if (!body.empty())
        body[body.size() - 1] += c;
}

void atm::Console::append_to_body_last_line(std::string msg)
{
    if (!body.empty())
        body[body.size() - 1] += msg;
}

void atm::Console::viewport_shift(bool up, size_t shift_by)
{
    if (up)
    {
        viewport.st -= shift_by;
        viewport.ed -= shift_by;
    }
    else
    {
        viewport.st += shift_by;
        viewport.ed += shift_by;
    }
    render();
}
