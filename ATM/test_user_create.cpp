#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <ctime>
#include "user.hpp"

namespace fs = std::filesystem;

// Function to create a user for testing
void create_user(accnum_t accnum, const std::string &user_name, double lim, size_t pin, double balance, accounts::__account_t account_type)
{
    // Construct user directory path
    fs::path user_path = fs::current_path() / "users" / std::to_string(accnum);

    // Create the directory if it doesn't exist
    if (!fs::exists(user_path))
    {
        fs::create_directories(user_path);
    }

    // Create Metadata.bin
    fs::path metadata_file = user_path / "Metadata.bin";
    std::ofstream metadata(metadata_file, std::ios::binary);
    if (metadata)
    {
        uint8_t account_type_byte = static_cast<uint8_t>(account_type);
        size_t username_length = user_name.size();
        std::time_t creation_date = std::time(nullptr); // Current time

        // Write metadata to the file
        metadata.write(reinterpret_cast<const char *>(&account_type_byte), sizeof(account_type_byte));
        metadata.write(reinterpret_cast<const char *>(&balance), sizeof(balance));
        metadata.write(reinterpret_cast<const char *>(&pin), sizeof(pin));
        metadata.write(reinterpret_cast<const char *>(&lim), sizeof(lim));
        metadata.write(reinterpret_cast<const char *>(&username_length), sizeof(username_length));
        metadata.write(user_name.data(), username_length);
        metadata.write(reinterpret_cast<const char *>(&creation_date), sizeof(creation_date));

        metadata.close();
        std::cout << "Metadata created for user: " << user_name << std::endl;
    }

    // Create Transactions.bin
    fs::path transactions_file = user_path / "Transactions.bin";
    std::ofstream transactions(transactions_file, std::ios::binary);
    transactions.close();
}

int main()
{
    bool continue_creating = true;
    while (continue_creating)
    {
        accnum_t accnum;
        std::string user_name;
        double lim;
        size_t pin;
        double balance;
        int account_type_choice;

        std::cout << "Enter Account Number: ";
        std::cin >> accnum;

        std::cin.ignore(); // Clear input buffer
        std::cout << "Enter User Name: ";
        std::getline(std::cin, user_name);

        std::cout << "Enter Transaction Limit: ";
        std::cin >> lim;

        std::cout << "Enter PIN: ";
        std::cin >> pin;

        std::cout << "Enter Initial Balance: ";
        std::cin >> balance;

        std::cout << "Select Account Type (0 - Saving, 1 - Checking, 2 - Business): ";
        std::cin >> account_type_choice;

        if (account_type_choice > 2 || account_type_choice < 0)        
        {
            std::cout << "Invalid account type: Using Saving as default.\n";
            account_type_choice = 0;
        }
        accounts::__account_t account_type = static_cast<accounts::__account_t>(account_type_choice);

        create_user(accnum, user_name, lim, pin, balance, account_type);

        char choice;
        std::cout << "Do you want to create another user? (y/n): ";
        std::cin >> choice;
        continue_creating = (choice == 'y' || choice == 'Y');
    }

    std::cout << "User creation completed." << std::endl;
    return 0;
}