#include "session.hpp"

bool atm::verify_account_num(std::string accnum)
{
    for (char c : accnum)
    {
        if (!(c >= '0' && c <= '9') && c != '.')
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
    c->print_to_rest("**************************************************");
    c->print_to_rest("*                WELCOME TO THE ATM              *");
    c->print_to_rest("**************************************************");
    c->print_to_rest("");
    c->print_to_rest("This program simulates a fully functional ATM system.");
    c->print_to_rest("With this simulation, you can perform the following tasks:");
    c->print_to_rest("  - Deposit funds into your account");
    c->print_to_rest("  - Withdraw funds from your account");
    c->print_to_rest("  - Transfer money to another account");
    c->print_to_rest("  - Check your account balance and details");
    c->print_to_rest("");
    c->print_to_rest("Additionally, you can log in and out of your account securely.");
    c->print_to_rest("For guidance, type 'help' at any time to view available commands.");
    c->print_to_rest("");
    c->print_to_rest("Let's begin!");
    c->print_to_rest("");
    c->print_to_rest("You must log in to access the ATM features.");
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
        else if (command == "withdraw" || command == "WITHDRAW")
            atm_withdraw();
        else if (command == "transfer" || command == "TRANSFER")
            atm_transfer();
        else if (command == "deposit" || command == "DEPOSIT")
            atm_deposit();
        else if (command == "clear" || command == "CLEAR")
            c->flush_body();
        else if (command == "BALANCE" || command == "balance")
        {
            double balance = API::get_balance();
            c->print_to_rest("Your current balance is: Rs" + std::to_string(balance));
            c->render();
        }
        else
        {
            c->flush_body();
            c->print_to_rest("Invalid Command!");
            c->render();
        }
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
    c->print_to_first_line("ATM Help Menu: Press 'q' to exit help.");
    c->print_to_rest("**************************************************");
    c->print_to_rest("*                 ATM COMMANDS                  *");
    c->print_to_rest("**************************************************");
    c->print_to_rest("");
    c->print_to_rest("Below is a list of all valid commands and their descriptions:");
    c->print_to_rest("");

    c->print_to_rest("HELP, help");
    c->print_to_rest("\tDisplay this help menu with details of available commands.");
    c->print_to_rest("");

    c->print_to_rest("LOGIN, login");
    c->print_to_rest("\tLog in to your account by entering your Account Number and PIN.");
    c->print_to_rest("");

    c->print_to_rest("LOGOUT, logout");
    c->print_to_rest("\tLog out of your current session securely.");
    c->print_to_rest("");

    c->print_to_rest("STATUS, status");
    c->print_to_rest("\tView detailed information about your account, including your balance.");
    c->print_to_rest("");

    c->print_to_rest("BALANCE, balance");
    c->print_to_rest("\tCheck your current account balance.");
    c->print_to_rest("");

    c->print_to_rest("WITHDRAW, withdraw");
    c->print_to_rest("\tWithdraw a specified amount from your account.");
    c->print_to_rest("");

    c->print_to_rest("DEPOSIT, deposit");
    c->print_to_rest("\tDeposit a specified amount into your account.");
    c->print_to_rest("");

    c->print_to_rest("TRANSFER, transfer");
    c->print_to_rest("\tTransfer funds to another account by specifying the target Account Number and amount.");
    c->print_to_rest("");

    c->print_to_rest("CLEAR, clear");
    c->print_to_rest("\tClear the screen content for better readability.");
    c->print_to_rest("");

    c->print_to_rest("QUIT, quit");
    c->print_to_rest("\tTerminate the ATM session and exit the program.");
    c->print_to_rest("");

    c->print_to_rest("**************************************************");
    c->print_to_rest("*         NAVIGATION AND ADDITIONAL NOTES       *");
    c->print_to_rest("**************************************************");
    c->print_to_rest("");
    c->print_to_rest("To navigate the screen history:");
    c->print_to_rest("  - Use the LEFT and RIGHT arrow keys to scroll.");
    c->print_to_rest("  - Use the UP and DOWN arrow keys to view previously entered commands.");
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
    std::time_t doc = API::get_doc();
    std::string doc_str = std::ctime(&doc);
    doc_str[doc_str.length() - 1] = 0;
    c->flush_body();
    c->print_to_rest("USERNAME: " + API::get_username());
    c->print_to_rest("ACCOUNT NUMBER: " + std::to_string(API::get_accnum()));
    c->print_to_rest("DATE OF CREATION: " + doc_str);
    c->print_to_rest("ACCOUNT TYPE: " + API::account_type_to_string(API::get_account_type()));
    c->print_to_rest("BALANCE: " + std::to_string(API::get_balance()));

    std::vector<accounts::Transaction> transactions = API::get_transaction_history();
    if (!transactions.empty())
    {
        c->print_to_rest("");
        c->print_to_rest("TRANSACTION HISTORY:");
    }
    for (accounts::Transaction t : transactions)
    {
        std::string ttime = std::ctime(&t.__time_of_transaction);
        ttime[ttime.length() - 1] = 0;
        switch (t.type)
        {
        case accounts::__transaction_t::__WITHDRAWING:
            c->print_to_rest("Withdrew Money...");
            c->print_to_rest("AMOUNT: " + std::to_string(t.__transaction_amount));
            c->print_to_rest("TIME: " + ttime);
            break;
        case accounts::__transaction_t::__DEPOSITING:
            c->print_to_rest("Deposited Money...");
            c->print_to_rest("AMOUNT: " + std::to_string(t.__transaction_amount));
            c->print_to_rest("TIME: " + ttime);
            break;
        case accounts::__transaction_t::__RECEIVING:
            c->print_to_rest("Received Money...");
            c->print_to_rest("SENDER: " + std::to_string(t.__sender));
            c->print_to_rest("AMOUNT: " + std::to_string(t.__transaction_amount));
            c->print_to_rest("TIME: " + ttime);
            break;
        case accounts::__transaction_t::__SENDING:
            c->print_to_rest("Sent Money...");
            c->print_to_rest("RECEIVER: " + std::to_string(t.__receiver));
            c->print_to_rest("AMOUNT: " + std::to_string(t.__transaction_amount));
            c->print_to_rest("TIME: " + ttime);
            break;
        }
        c->print_to_rest("");
    }

    c->render();
}

void atm::atm_deposit()
{
    if (!API::session_active())
    {
        c->flush_body();
        c->print_to_rest("Login first to perform any operations!");
        c->render();
        return;
    }
    std::string amount;
    c->flush_body();
    // Mimic amount being inserted
    c->print_to_rest("ENTER THE AMOUNT TO DEPOSIT: ");
    amount = atm::input();
    if (!atm::verify_account_num(amount))
    {
        c->flush_body();
        c->print_to_rest("Invalid amount entered.");
        c->render();
        return;
    }
    if (!API::deposit(std::strtod(amount.c_str(), NULL)))
    {
        c->flush_body();
        c->print_to_rest("Failed to deposit....");
        c->print_to_rest(API::get_tmp_msg());
        c->render();
        return;
    }
    c->flush_body();
    c->print_to_rest("Deposit successful!");
    c->print_to_rest("Use the status command to confirm!");
    c->render();
}

void atm::atm_withdraw()
{
    if (!API::session_active())
    {
        c->flush_body();
        c->print_to_rest("Login first to perform any operations!");
        c->render();
        return;
    }
    std::string amount;
    c->flush_body();
    // Mimic amount being taken
    c->print_to_rest("ENTER THE AMOUNT TO WITHDRAW: ");
    amount = atm::input();
    if (!atm::verify_account_num(amount))
    {
        c->flush_body();
        c->print_to_rest("Invalid amount entered.");
        c->render();
        return;
    }
    if (!API::withdraw(std::strtod(amount.c_str(), NULL)))
    {
        c->flush_body();
        c->print_to_rest("Failed to withdraw....");
        c->print_to_rest(API::get_tmp_msg());
        c->render();
        return;
    }
    c->flush_body();
    c->print_to_rest("Withdraw successful!");
    c->print_to_rest("Use the status command to confirm!");
    c->render();
}

void atm::atm_transfer()
{
    if (!API::session_active())
    {
        c->flush_body();
        c->print_to_rest("Login first to perform any operations!");
        c->render();
        return;
    }
    std::string amount, receiver;
    c->flush_body();
    // Mimic amount being taken
    c->print_to_rest("ENTER THE RECEIVER'S ACCOUNT NUMBER: ");
    receiver = atm::input();
    if (!atm::verify_account_num(receiver))
    {
        c->flush_body();
        c->print_to_rest("Invalid account number entered.");
        c->render();
        return;
    }
    c->print_to_rest("ENTER THE AMOUNT TO SEND: ");
    amount = atm::input();
    if (!atm::verify_account_num(amount))
    {
        c->flush_body();
        c->print_to_rest("Invalid amount entered.");
        c->render();
        return;
    }
    if (!API::transfer(strtoul(receiver.c_str(), NULL, 10), strtod(amount.c_str(), NULL)))
    {
        c->flush_body();
        c->print_to_rest("Failed to transfer....");
        c->print_to_rest(API::get_tmp_msg());
        c->render();
        return;
    }
    c->flush_body();
    c->print_to_rest("Transfer successful!");
    c->print_to_rest("Use the status command to confirm!");
    c->render();
}