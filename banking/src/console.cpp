#include "console.hpp"

bank::Console *bank::console()
{
    return &__console;
}

void bank::Console::setup_console()
{
    // prepare the console
    cl_init();
    setup_resize_handler();
    std::cout.setf(std::ios::unitbuf); // disable buffering for output
    update_viewport();
}

void bank::Console::cl_init()
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

void bank::Console::cl_reset()
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

void bank::Console::console_println(std::string msg)
{
    // specifically for writing when in raw mode
    std::cout << msg << "\r\n";
}

void bank::Console::console_print(std::string msg)
{
    // specifically for writing when in raw mode
    std::cout << msg;
}

bank::Console::~Console()
{
    cl_reset();
}

void bank::Console::print_to_first_line(std::string msg)
{
    first_line += msg;
}

void bank::Console::print_to_first_line(char c)
{
    first_line += c;
}

void bank::Console::print_to_rest(std::string msg)
{
    // Each message is going to be one line long
    body.push_back(msg);
}

void bank::Console::print_to_rest(char c)
{
    body.push_back(std::string({c}));
}

void bank::Console::remove_from_fist_line()
{
    if (!first_line.empty())
        first_line.pop_back();
}

void bank::Console::flush_first_line()
{
    first_line.erase();
}

void bank::Console::overwrite_first_line(std::string msg)
{
    first_line = msg;
}

void bank::Console::render()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    size_t lines_to_print = 0;
    size_t padding_lines = 0;

    if (body.size() == 0)
    {
        lines_to_print = 0;
        padding_lines = viewport.ed;
    }
    else if (body.size() - st_ind > viewport.ed)
    {
        lines_to_print = viewport.ed;
    }
    else
    {
        lines_to_print = body.size() - st_ind;
        padding_lines = viewport.ed - lines_to_print;
    }

    // Print the lines in the viewport range
    for (size_t i = 0; i < lines_to_print; ++i)
    {
        console_println(body[st_ind + i]);
    }

    // Add padding lines if necessary
    for (size_t i = 0; i < padding_lines; ++i)
    {
        console_println("");
    }

    // Print the first line
    console_print(first_line);
}

void bank::Console::flush()
{
    first_line.erase();
    body.erase(body.begin(), body.end());
    update_viewport();
}

int bank::Console::get_console_rows()
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
BOOL WINAPI bank::handle_resize(DWORD rows)
#else
void bank::handle_resize(int rows)
#endif
{
    __console.update_viewport();
    __console.render();
}

void bank::Console::update_viewport()
{
    int row_count = __console.get_console_rows();
    viewport.st = 0;
    viewport.ed = row_count - 1;
    // we don't do anything else here
}

void bank::Console::flush_body()
{
    body.erase(body.begin(), body.end());
    update_viewport();
    st_ind = 0;
}

void bank::Console::setup_resize_handler()
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

void bank::Console::append_to_body_last_line(char c)
{
    if (!body.empty())
        body[body.size() - 1] += c;
}

void bank::Console::append_to_body_last_line(std::string msg)
{
    if (!body.empty())
        body[body.size() - 1] += msg;
}

void bank::Console::viewport_shift(bool up, size_t shift_by)
{
    if (up)
    {
        if (st_ind > 0)
            st_ind--;
    }
    else
    {
        if (st_ind < (body.size() - 1))
            st_ind++;
    }
    render();
}
