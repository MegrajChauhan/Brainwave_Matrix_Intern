#include "session.hpp"

bool atm::verify_account_num(std::string accnum)
{
    for (char c : accnum)
    {
        if (!(c >= '0' && c <= '9'))
            return false;
    }
    return true;
}

void atm::preinit()
{
    c = console();
    c->setup_console();
}

void atm::start_atm()
{
    // Firstly, we need to welcome the user
    c->print_to_rest("Welcome to the ATM Simulation!");
    c->print_to_rest("This program provides a simple simulation of basic ATM functionalities.");
    c->print_to_rest("You can perform common tasks like deposits, withdrawals, and balance inquiries.");
    c->print_to_rest("You can perform common tasks like deposits, withdrawals, and balance inquiries.");
    c->print_to_rest("Let's get started!");
    c->print_to_rest("");
    c->print_to_rest("You need to login first.");
    c->print_to_first_line("Press any key to continue...");
    c->render();
    wait();
    c->flush();
    c->render();
    std::string command;
    bool terminate = false;
    while (!terminate)
    {
        command = input();
        if (command == "QUIT" || command == "Q" || command == "q" || command == "quit")
        {
            terminate = true;
            c->flush();
            c->print_to_rest("Terminating....");
            c->render();
        }
        else if (command == "login" || command == "LOGIN")
            atm_login();
        else if (command == "logout" || command == "LOGOUT")
            atm_logout();
        else{
            c->flush_body();
            c->print_to_rest("Invalid Command!");
            c->render();
        }
        // else if (command == "DEPOSIT" || command == "deposit")
        // {

        //     c->print_to_rest("Enter the amount to deposit: ");
        //     c->render();
        //     std::string amount_str = input();
        //     double amount = std::stod(amount_str);
        //     if (API::deposit(amount))
        //     {
        //         c->print_to_rest("Deposit successful!");
        //     }
        //     else
        //     {
        //         c->print_to_rest("Deposit failed. Please check your input.");
        //     }
        //     c->render();
        // }
        // else if (command == "WITHDRAW" || command == "withdraw")
        // {
        //     c->print_to_rest("Enter the amount to withdraw: ");
        //     c->render();
        //     std::string amount_str = input();
        //     double amount = std::stod(amount_str);
        //     if (API::withdraw(amount))
        //     {
        //         c->print_to_rest("Withdrawal successful!");
        //     }
        //     else
        //     {
        //         c->print_to_rest("Withdrawal failed. Insufficient balance or invalid input.");
        //     }
        //     c->render();
        // }
        // else if (command == "BALANCE" || command == "balance")
        // {
        //     double balance = API::get_balance();
        //     c->print_to_rest("Your current balance is: $" + std::to_string(balance));
        //     c->render();
        // }
        // else
        // {
        //     c->print_to_rest("Invalid command. Please try again.");
        //     c->render();
        // }
    }
}

bool atm::atm_login()
{
    accnum_t num;
    size_t pin;
    if (API::session_active())
    {
        c->flush_body();
        c->print_to_rest("A session is already running....");
        c->print_to_rest("Log out first");
        c->render();
        return false;
    }
    c->flush();
    c->print_to_rest("ACCOUNT NUMBER: ");
    std::string command = input(); // read in the account number
    if (!verify_account_num(command))
    {
        c->flush_body();
        c->print_to_rest("ERROR: Invalid account number given.");
        c->render(); // re-try
        return false;
    }
    else
        num = std::strtoul(command.c_str(), NULL, 10);
    reverse_secret();
    c->print_to_rest("PIN: ");
    reverse_secret();
    command = input();

    // don't mind the name
    if (!verify_account_num(command))
    {
        c->flush_body();
        c->print_to_rest("ERROR: Invalid PIN given.");
        c->render(); // re-try
        return false;
    }
    else
        pin = std::strtoul(command.c_str(), NULL, 10);

    if (!API::log_in(num, pin))
    {
        c->flush_body();
        c->print_to_rest("Failed to login!");
        c->render();
        return false;
    }
    return true;
}

bool atm::atm_logout()
{
    if (API::session_active())
        API::log_out();
    else
    {
        c->flush_body();
        c->print_to_rest("Failed to log-out(Login First!)");
        c->render();
        return false;
    }
    return true;
}