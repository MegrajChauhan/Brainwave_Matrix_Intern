#include "console.hpp"

int main()
{
    atm::Console console;
    console.setup_console();
    std::string input = console.get_input();
    return 0;
}