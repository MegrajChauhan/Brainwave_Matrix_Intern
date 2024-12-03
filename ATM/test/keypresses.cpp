#include <iostream>
#include <unistd.h>
#include <termios.h>

void set_raw_mode()
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO | ICANON); // Disable echo and canonical mode
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void reset_mode()
{
    struct termios original;
    tcgetattr(STDIN_FILENO, &original);
    original.c_lflag |= (ECHO | ICANON); // Restore previous settings
    tcsetattr(STDIN_FILENO, TCSANOW, &original);
}

int main()
{
    set_raw_mode();
    std::cout.setf(std::ios::unitbuf);
    while (true)
    {
        char c[255];
        ssize_t size = read(STDIN_FILENO, c, 255);

        if (size > 0)
        {
            if (size == 1)
                std::cout << "Key code: " << static_cast<int>(c[0]) << " ('" << c << "')\r\n";
            else
            {
                for (int i = 0; i < size; i++)
                {
                    std::cout << "Key code: " << static_cast<int>(c[i]) << " ('" << c[i] << "')\r\n";
                }
                std::cout<<"\r\n";
            }
        }
    }

    reset_mode();
    return 0;
}
