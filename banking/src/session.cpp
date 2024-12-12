#include "session.hpp"

bool bank::verify_account_num(std::string accnum)
{
    for (char c : accnum)
    {
        if (!(c >= '0' && c <= '9') && c != '.')
            return false;
    }
    return true;
}

void bank::preinit()
{
    c = console();
    c->setup_console();
}

void bank::start_bank()
{
    // Firstly, we need to welcome the user
    c->print_to_rest("**************************************************");
    c->print_to_rest("*                WELCOME TO THE BANK             *");
    c->print_to_rest("**************************************************");
    c->print_to_rest("");
    c->print_to_rest("This program simulates a fully functional banking system.");
    c->print_to_rest("With this simulation, you can perform the following tasks:");
    c->print_to_rest("  - Deposit funds into your account");
    c->print_to_rest("  - Withdraw funds from your account");
    c->print_to_rest("  - Transfer money to another account");
    c->print_to_rest("  - Check your account balance and details");
    c->print_to_rest("  - Register a new account");
    c->print_to_rest("");
    c->print_to_rest("Additionally, you can log in and out of your account securely.");
    c->print_to_rest("For guidance, type 'help' at any time to view available commands.");
    c->print_to_rest("");
    c->print_to_rest("Let's begin!");
    c->print_to_rest("");
    c->print_to_rest("You must log in to access the bank features.");
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
            bank_login();
        else if (command == "logout" || command == "LOGOUT")
            bank_logout();
        else if (command == "help" || command == "HELP")
            bank_help();
        else if (command == "status" || command == "STATUS")
            bank_print_user_details();
        else if (command == "withdraw" || command == "WITHDRAW")
            bank_withdraw();
        else if (command == "transfer" || command == "TRANSFER")
            bank_transfer();
        else if (command == "deposit" || command == "DEPOSIT")
            bank_deposit();
        else if (command == "clear" || command == "CLEAR")
            c->flush_body();
        else if (command == "register" || command == "REGISTER")
            bank_register_user();
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

bool bank::bank_login()
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
    c->print_to_rest("Account Number: ");
    std::string command = input();
    if (!verify_account_num(command))
    {
        c->flush_body();
        c->print_to_rest("ERROR: Invalid account number given.");
        c->render();
        return false;
    }
    else
        num = std::strtoul(command.c_str(), NULL, 10);
    c->print_to_first_line(command);
    reverse_secret();
    c->print_to_rest("Pin: ");
    command = input();
    reverse_secret();

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

bool bank::bank_logout()
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

void bank::bank_help()
{
    c->flush();
    c->print_to_first_line("Bank Help Menu: Press 'q' to exit help.");
    c->print_to_rest("**************************************************");
    c->print_to_rest("*                 BANK COMMANDS                  *");
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

    c->print_to_rest("REGISTER, register");
    c->print_to_rest("\tRegister a new user.");
    c->print_to_rest("");

    c->print_to_rest("QUIT, quit");
    c->print_to_rest("\tTerminate the bank session and exit the program.");
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
    while ((curr = bank::input_no_show()) != "q")
        ;
}

void bank::bank_print_user_details()
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
    c->print_to_rest("Username        : " + API::get_username());
    c->print_to_rest("Account Number  : " + std::to_string(API::get_accnum()));
    c->print_to_rest("Date of Creation: " + doc_str);
    c->print_to_rest("Account Type    : " + API::account_type_to_string(API::get_account_type()));
    c->print_to_rest("Balance         : Rs" + std::to_string(API::get_balance()));

    std::vector<accounts::Transaction> transactions = API::get_transaction_history();
    if (!transactions.empty())
    {
        c->print_to_rest("");
        c->print_to_rest("Transaction History:");
    }
    for (accounts::Transaction t : transactions)
    {
        std::string ttime = std::ctime(&t.__time_of_transaction);
        ttime[ttime.length() - 1] = 0;
        switch (t.type)
        {
        case accounts::__transaction_t::__WITHDRAWING:
            c->print_to_rest("Withdrew Money...");
            c->print_to_rest("Amount: " + std::to_string(t.__transaction_amount));
            c->print_to_rest("Time  : " + ttime);
            break;
        case accounts::__transaction_t::__DEPOSITING:
            c->print_to_rest("Deposited Money...");
            c->print_to_rest("Amount: " + std::to_string(t.__transaction_amount));
            c->print_to_rest("Time  : " + ttime);
            break;
        case accounts::__transaction_t::__RECEIVING:
            c->print_to_rest("Received Money...");
            c->print_to_rest("Sender: " + std::to_string(t.__sender));
            c->print_to_rest("Amount: " + std::to_string(t.__transaction_amount));
            c->print_to_rest("Time  : " + ttime);
            break;
        case accounts::__transaction_t::__SENDING:
            c->print_to_rest("Sent Money...");
            c->print_to_rest("Receiver: " + std::to_string(t.__receiver));
            c->print_to_rest("Amount  : " + std::to_string(t.__transaction_amount));
            c->print_to_rest("Time    : " + ttime);
            break;
        }
        c->print_to_rest("");
    }

    c->render();
}

void bank::bank_deposit()
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
    c->print_to_rest("Enter the amount to deposit: ");
    amount = bank::input();
    if (!bank::verify_account_num(amount))
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

void bank::bank_withdraw()
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
    c->print_to_rest("Enter the amount to withdraw: ");
    amount = bank::input();
    if (!bank::verify_account_num(amount))
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

void bank::bank_transfer()
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
    c->print_to_rest("Enter the receiver's account number: ");
    receiver = bank::input();
    if (!bank::verify_account_num(receiver))
    {
        c->flush_body();
        c->print_to_rest("Invalid account number entered.");
        c->render();
        return;
    }
    c->print_to_rest("Enter the ammount to send: ");
    amount = bank::input();
    if (!bank::verify_account_num(amount))
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

// Function to generate a unique account number
unsigned long long bank::generate_account_number()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    std::mt19937 rng(static_cast<unsigned>(millis));

    std::uniform_int_distribution<int> dist(1000, 9999);
    int random_component = dist(rng);

    std::stringstream ss;
    ss << millis << random_component;

    return std::strtoull(ss.str().c_str(), nullptr, 10);
}

void bank::bank_register_user()
{
    c->flush_body();
    c->print_to_rest("=== USER REGISTRATION ===");

    c->print_to_rest("Enter your name: ");
    std::string name = bank::input();

    c->print_to_rest("Enter a 4-digit PIN: ");
    std::string pin;
    do
    {
        pin = bank::input();
        if (pin.size() != 4 || !std::all_of(pin.begin(), pin.end(), ::isdigit))
        {
            c->print_to_rest("Invalid PIN. Enter a 4-digit numeric PIN: ");
        }
    } while (pin.size() != 4 || !std::all_of(pin.begin(), pin.end(), ::isdigit));

    accnum_t account_number = generate_account_number();

    std::string acc_type;
    c->print_to_rest("Enter the type of account(0: Savings, 1: Checking. 2: Business): ");
    accounts::__account_t t;
    do
    {
        acc_type = bank::input();
        if (acc_type == "0")
        {
            t = accounts::__SAVING;
            break;
        }
        else if (acc_type == "1")
        {
            t = accounts::__CHECKING;
            break;
        }
        else if (acc_type == "2")
        {
            t = accounts::__BUSINESS_ACCOUNT;
            break;
        }
        else
        {
            c->print_to_rest("Invalid Account Type!");
        }
    } while (true);

    if (!API::register_user(account_number, name, std::stoul(pin), t))
    {
        c->flush_body();
        c->print_to_rest("Failed to register user. Try again later.");
        c->print_to_rest(API::get_tmp_msg());
        c->render();
        return;
    }

    // Success
    c->flush_body();
    c->print_to_rest("Registration successful!");
    c->print_to_rest("Your account number is: " + account_number);
    c->render();
}