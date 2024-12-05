#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <ctime>
#include "user.hpp"

namespace fs = std::filesystem;

// Function to create a user for testing
void create_user(accnum_t accnum, const std::string &user_name, size_t pin, double balance, accounts::__account_t account_type)
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
        metadata.write(reinterpret_cast<const char *>(&username_length), sizeof(username_length));
        metadata.write(user_name.data(), username_length);
        metadata.write(reinterpret_cast<const char *>(&creation_date), sizeof(creation_date));

        metadata.close();
        std::cout << "Metadata created for user: " << user_name << std::endl;
    }

    // Create Transactions.bin
    fs::path transactions_file = user_path / "Transactions.bin";
    std::ofstream transactions(transactions_file, std::ios::binary);
    if (transactions)
    {
        // Example transaction data for testing
        std::vector<accounts::Transaction> transaction_list = {
            accounts::Transaction(accnum, 123456, 50.0, std::time(nullptr), accounts::__transaction_t::__SENDING),
            accounts::Transaction(123456, accnum, 100.0, std::time(nullptr), accounts::__transaction_t::__RECEIVING)};

        // Write transactions to the file
        for (const auto &trans : transaction_list)
        {
            transactions.write(reinterpret_cast<const char *>(&trans.__sender), sizeof(accnum_t));
            transactions.write(reinterpret_cast<const char *>(&trans.__receiver), sizeof(accnum_t));
            transactions.write(reinterpret_cast<const char *>(&trans.__transaction_amount), sizeof(double));
            uint8_t type_byte = static_cast<uint8_t>(trans.type);
            transactions.write(reinterpret_cast<const char *>(&type_byte), sizeof(type_byte));
            transactions.write(reinterpret_cast<const char *>(&trans.__time_of_transaction), sizeof(std::time_t));
        }

        transactions.close();
        std::cout << "Transactions created for user: " << user_name << std::endl;
    }
}

int main()
{
    // Example usage of create_user function
    create_user(1001, "Alice", 1234, 5000.0, accounts::__account_t::__SAVING);
    create_user(1002, "Bob", 5678, 3000.0, accounts::__account_t::__CHECKING);
    create_user(1003, "Charlie", 9876, 10000.0, accounts::__account_t::__BUSINESS_ACCOUNT);
    create_user(1233, "Pradip", 1100, 10000000000.0, accounts::__account_t::__SAVING);

    std::cout << "User creation completed." << std::endl;
    return 0;
}
