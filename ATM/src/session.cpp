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
        else if (command == "help" || command == "HELP")
            atm_help();
        else if (command == "status" || command == "STATUS")
            atm_print_user_details();
        else
        {
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
    c->print_to_first_line(command);
    reverse_secret();
    c->print_to_rest("PIN: ");
    command = input();
    reverse_secret();

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
    c->flush_body();
    c->print_to_rest("Successfully logged in as " + API::get_username());
    c->render();
    return true;
}

bool atm::atm_logout()
{
    if (API::session_active())
    {
        c->flush_body();
        c->print_to_rest("Successfully logged out as " + API::get_username());
        API::log_out();
        c->render();
    }
    else
    {
        c->flush_body();
        c->print_to_rest("Failed to log-out(Login First!)");
        c->render();
        return false;
    }
    return true;
}

void atm::atm_help()
{
    c->flush();
    c->print_to_first_line("Press left-arrow to move down, right-arrow to move up else any key to exit");
    c->print_to_rest("Navigation");
    c->print_to_rest("In order to navigate the screen, if you cannot see anything than you can use the left and right arrow keys to navigate.");
    c->print_to_rest("");
    c->print_to_rest("The UP and DOWN arrow keys will help you navigate all of the commands you have previously entered.");
    c->print_to_rest("");
    c->print_to_rest("A list of all possible commands:");
    c->print_to_rest("HELP, help");
    c->print_to_rest("\tDisplay this help message");
    c->print_to_rest("");
    c->print_to_rest("LOGIN, login");
    c->print_to_rest("\tLogin as a user by entering your Account number and password");
    c->print_to_rest("");
    c->print_to_rest("LOGOUT, logout");
    c->print_to_rest("\tLogout as the current user");
    c->print_to_rest("");
    c->render();
    std::string curr;
    while ((curr = atm::input_no_show()) != "q")
        ;
}

void atm::atm_print_user_details()
{
    if (!API::session_active())
    {
        c->flush_body();
        c->print_to_rest("Login first to see details");
        c->render();
        return;
    }
    c->flush_body();
    c->print_to_rest("USERNAME: " + API::get_username());
    c->print_to_rest("ACCOUNT NUMBER: " + std::to_string(API::get_accnum()));
    c->print_to_rest("DATE OF CREATION: ");
    c->print_to_rest("ACCOUNT TYPE: " + API::account_type_to_string(API::get_account_type()));
    c->print_to_rest("BALANCE: " + std::to_string(API::get_balance()));
    c->render();
}