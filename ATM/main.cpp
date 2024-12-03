#include "console.hpp"
#include "IO.hpp"

void show_welcome_message()
{
    atm::Console *console = atm::console();
    console->print_to_rest(
        "Welcome to the ATM Simulation!\n\r"
        "This program provides a simple simulation of basic ATM functionalities.\n\r"
        "You can perform common tasks like deposits, withdrawals, and balance inquiries.\n\r"
        "Let's get started!");
}

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