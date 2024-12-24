#include "fetch.hpp"

bool fetch::user_exists(accnum_t accnum)
{
    fs::path user_path = fs::current_path() / std::to_string(accnum);
    return fs::exists(user_path) && fs::is_directory(user_path);
}

accounts::User fetch::fetch_user(accnum_t accnum)
{
    _status = true;
    accounts::User user;

#ifdef _WIN32
    fs::path user_path = fs::current_path() / "..\atm_users"/ std::to_string(accnum);
#else
    fs::path user_path = fs::current_path() / "../atm_users" / std::to_string(accnum);
#endif
    if (!fs::exists(user_path) || !fs::is_directory(user_path))
    {
        _status = false;
        return user;
    }

    uint8_t account_type_byte;
    double balance = 0;
    size_t pin = 0;
    size_t username_length = 0;
    std::string user_name;
    std::time_t creation_date;
    std::vector<accounts::Transaction> transactions_list;
    accounts::__account_t account_type;
    double tlim = 0;

    fs::path metadata_file = user_path / "Metadata.bin";
    if (fs::exists(metadata_file))
    {
        std::fstream metadata;
        metadata.open(metadata_file.string(), std::ios::binary | std::ios::in);
        if (metadata.is_open())
        {
            metadata.read(reinterpret_cast<char *>(&account_type_byte), sizeof(account_type_byte));
            account_type = static_cast<accounts::__account_t>(account_type_byte);

            metadata.read(reinterpret_cast<char *>(&balance), sizeof(balance));
            metadata.read(reinterpret_cast<char *>(&pin), sizeof(pin));

            metadata.read(reinterpret_cast<char *>(&tlim), sizeof(double));

            metadata.read(reinterpret_cast<char *>(&username_length), 8);

            char name[username_length + 1];
            metadata.read(name, sizeof(name));
            name[username_length] = 0;
            user_name = name;

            metadata.read(reinterpret_cast<char *>(&creation_date), sizeof(creation_date));
        }
        metadata.close();
    }

    fs::path transactions_file = user_path / "Transactions.bin";
    if (fs::exists(transactions_file))
    {
        std::fstream transactions;
        transactions.open(transactions_file.string(), std::ios::binary | std::ios::in);
        if (transactions.is_open())
        {
            while (transactions.peek() != EOF)
            {
                accounts::Transaction trans;

                transactions.read(reinterpret_cast<char *>(&trans.__sender), sizeof(accnum_t));
                transactions.read(reinterpret_cast<char *>(&trans.__receiver), sizeof(accnum_t));

                transactions.read(reinterpret_cast<char *>(&trans.__transaction_amount), sizeof(double));

                uint8_t type_byte;
                transactions.read(reinterpret_cast<char *>(&type_byte), sizeof(type_byte));
                trans.type = static_cast<accounts::__transaction_t>(type_byte);

                transactions.read(reinterpret_cast<char *>(&trans.__time_of_transaction), sizeof(std::time_t));

                transactions_list.push_back(trans);
            }
        }
        transactions.close();
    }
    user = accounts::User(transactions_list, tlim, pin, user_name, accnum, balance, account_type, creation_date);
    return user;
}

bool fetch::fetch_status()
{
    return _status;
}

bool fetch::unfetch_user(accounts::User user)
{
    _status = true;
#ifdef _WIN32
    fs::path user_path = fs::current_path() / "..\atm_users"/ std::to_string(user.get_accnum());
#else
    fs::path user_path = fs::current_path() / "../atm_users" / std::to_string(user.get_accnum());
#endif

    if (!fs::exists(user_path) || !fs::is_directory(user_path))
    {
        return false;
    }

    fs::path metadata_file = user_path / "Metadata.bin";
    {
        std::fstream metadata;
        metadata.open(metadata_file.string(), std::ios::binary | std::ios::out | std::ios::trunc);
        if (!metadata.is_open())
        {
            return false;
        }

        uint8_t account_type_byte = static_cast<uint8_t>(user.get_account_type());
        metadata.write(reinterpret_cast<char *>(&account_type_byte), sizeof(account_type_byte));

        double balance = user.get_balance();
        metadata.write(reinterpret_cast<char *>(&balance), sizeof(balance));

        size_t pin = user.get_pin();
        metadata.write(reinterpret_cast<char *>(&pin), sizeof(pin));

        double lim = user.get_lim();
        metadata.write(reinterpret_cast<char *>(&lim), sizeof(lim));

        std::string username = user.get_username();
        size_t username_length = username.length();
        metadata.write(reinterpret_cast<char *>(&username_length), sizeof(username_length));
        metadata.write(username.c_str(), username_length);

        std::time_t creation_date = user.get_doc();
        metadata.write(reinterpret_cast<char *>(&creation_date), sizeof(creation_date));

        metadata.close();
    }

    fs::path transactions_file = user_path / "Transactions.bin";
    {
        std::fstream transactions;
        transactions.open(transactions_file.string(), std::ios::binary | std::ios::out | std::ios::trunc);
        if (!transactions.is_open())
        {
            return false;
        }

        const auto &transactions_list = user.get_transaction_history();
        for (const auto &trans : transactions_list)
        {
            transactions.write(reinterpret_cast<const char *>(&trans.__sender), sizeof(accnum_t));
            transactions.write(reinterpret_cast<const char *>(&trans.__receiver), sizeof(accnum_t));

            transactions.write(reinterpret_cast<const char *>(&trans.__transaction_amount), sizeof(double));

            uint8_t type_byte = static_cast<uint8_t>(trans.type);
            transactions.write(reinterpret_cast<const char *>(&type_byte), sizeof(type_byte));

            transactions.write(reinterpret_cast<const char *>(&trans.__time_of_transaction), sizeof(std::time_t));
        }

        transactions.close();
    }

    return true; // Successfully wrote all changes
}
