#include "console.hpp"
#include "IO.hpp"

int main()
{
    atm::console()->setup_console();
    std::string inp = atm::input();
    return 0;
}