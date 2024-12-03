#include "console.hpp"
#include "IO.hpp"

int main()
{
    atm::console()->setup_console();
    std::string inp = atm::input();
    while (inp != "quit")
    {
        if (inp == "history")
        {
            atm::show_history();
        }
        inp = atm::input();
    }
    atm::println("");
    return 0;
}