#include "fetch.hpp"

bool fetch::user_exists(accnum_t accnum)
{
    fs::path user_path = fs::current_path() / std::to_string(accnum);
    return fs::exists(user_path) && fs::is_directory(user_path);
}

accounts::User fetch::fetch_user(accnum_t accnum)
{
    fs::path user_path = fs::current_path() / "users" / std::to_string(accnum);
    if (!fs::exists(user_path) || !fs::is_directory(user_path))
    {
        throw std::runtime_error("User with account number " + std::to_string(accnum) + " does not exist.");
    }

    accounts::User user;
    uint8_t account_type_byte;
    size_t balance = 0, pin = 0;
    size_t username_length = 0;
    std::string user_name;
    std::time_t creation_date;
    std::vector<accounts::Transaction> transactions_list;
    accounts::__account_t account_type;

    fs::path metadata_file = user_path / "Metadata.bin";
    if (fs::exists(metadata_file))
    {
        std::ifstream metadata(metadata_file, std::ios::binary);
        if (metadata)
        {
            metadata.read(reinterpret_cast<char *>(&account_type_byte), sizeof(account_type_byte));
            account_type = static_cast<accounts::__account_t>(account_type_byte);

            metadata.read(reinterpret_cast<char *>(&balance), sizeof(balance));
            metadata.read(reinterpret_cast<char *>(&pin), sizeof(pin));

            metadata.read(reinterpret_cast<char *>(&username_length), 2);

            char name[username_length];
            metadata.read(name, sizeof(name));
            user_name = name;

            metadata.read(reinterpret_cast<char *>(&creation_date), sizeof(creation_date));
        }
    }

    fs::path transactions_file = user_path / "Transactions.bin";
    if (fs::exists(transactions_file))
    {
        std::ifstream transactions(transactions_file, std::ios::binary);
        if (transactions)
        {

            while (transactions.peek() != EOF)
            {
                accounts::Transaction trans;

                transactions.read(reinterpret_cast<char *>(&trans.__sender), sizeof(accnum_t));
                transactions.read(reinterpret_cast<char *>(&trans.__receiver), sizeof(accnum_t));

                transactions.read(reinterpret_cast<char *>(&trans.__transaction_amount), sizeof(size_t));

                uint8_t type_byte;
                transactions.read(reinterpret_cast<char *>(&type_byte), sizeof(type_byte));
                trans.type = static_cast<accounts::__transaction_t>(type_byte);

                transactions.read(reinterpret_cast<char *>(&trans.__time_of_transaction), sizeof(std::time_t));

                transactions_list.push_back(trans);
            }
        }
    }
    user = accounts::User(transactions_list, 0, pin, user_name, accnum, balance, account_type, creation_date);
    return user;
}
