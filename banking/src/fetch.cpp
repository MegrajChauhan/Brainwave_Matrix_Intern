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
    fs::path user_path = fs::current_path() / "..\users" / std::to_string(accnum);
#else
    fs::path user_path = fs::current_path() / "../users" / std::to_string(accnum);
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
    std::time_t creation_date = 0;
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

bool fetch::unfetch_user(accounts::User *user)
{
    _status = true;

#ifdef _WIN32
    fs::path user_path = fs::current_path() / "..\users" / std::to_string(user->get_accnum());
#else
    fs::path user_path = fs::current_path() / "../users" / std::to_string(user->get_accnum());
#endif

    if (!fs::exists(user_path) || !fs::is_directory(user_path))
    {
        return false;
    }

    fs::path metadata_file = user_path / "Metadata.bin";
    {
        std::fstream metadata;
        metadata.open(metadata_file.string(), std::ios::binary | std::ios::out);
        if (!metadata.is_open())
        {
            return false;
        }

        uint8_t account_type_byte = static_cast<uint8_t>(user->get_account_type());
        metadata.write(reinterpret_cast<char *>(&account_type_byte), sizeof(account_type_byte));

        double balance = user->get_balance();
        metadata.write(reinterpret_cast<char *>(&balance), sizeof(balance));

        size_t pin = user->get_pin();
        metadata.write(reinterpret_cast<char *>(&pin), sizeof(pin));

        double lim = user->get_lim();
        metadata.write(reinterpret_cast<char *>(&lim), sizeof(lim));

        std::string username = user->get_username();
        size_t username_length = username.length();
        metadata.write(reinterpret_cast<char *>(&username_length), sizeof(username_length));
        metadata.write(username.c_str(), username_length);

        std::time_t creation_date = user->get_doc();
        metadata.write(reinterpret_cast<char *>(&creation_date), sizeof(creation_date));

        metadata.close();
    }

    fs::path transactions_file = user_path / "Transactions.bin";
    {
        std::fstream transactions;
        transactions.open(transactions_file.string(), std::ios::binary | std::ios::out);
        if (!transactions.is_open())
        {
            return false;
        }

        const auto &transactions_list = user->get_transaction_history();
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

bool fetch::create_user(accnum_t accnum, std::string name, size_t pin, accounts::__account_t type)
{
#ifdef _WIN32
    fs::path user_path = fs::current_path() / "..\users" / std::to_string(accnum);
#else
    fs::path user_path = fs::current_path() / "../users" / std::to_string(accnum);
#endif

    if (fs::exists(user_path))
        return false;

    fs::create_directories(user_path);

    fs::path metadata_file = user_path / "Metadata.bin";
    std::ofstream metadata(metadata_file.string(), std::ios::binary | std::ios::out);
    if (!metadata.is_open())
        return false;

    uint8_t account_type_byte = static_cast<uint8_t>(type);
    double initial_balance = 0.0;
    std::time_t creation_date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    size_t username_length = name.length();

    double limit = type == accounts::__account_t::__SAVING ? 10000 : type == accounts::__account_t::__CHECKING ? 100000
                                                                                                               : 900000;

    metadata.write(reinterpret_cast<const char *>(&account_type_byte), sizeof(account_type_byte));
    metadata.write(reinterpret_cast<const char *>(&initial_balance), sizeof(initial_balance));
    metadata.write(reinterpret_cast<const char *>(&pin), sizeof(pin));

    metadata.write(reinterpret_cast<const char *>(&limit), sizeof(double));

    metadata.write(reinterpret_cast<const char *>(&username_length), sizeof(username_length));
    metadata.write(name.c_str(), username_length);
    metadata.write(reinterpret_cast<const char *>(&creation_date), sizeof(creation_date));

    metadata.close();

    fs::path transactions_file = user_path / "Transactions.bin";
    std::ofstream transactions(transactions_file.string(), std::ios::binary | std::ios::out);
    if (!transactions.is_open())
        return false;

    transactions.close();

    return true;
}

bool fetch::fetch_user_metadata(accnum_t accnum, std::string *buf)
{
#ifdef _WIN32
    fs::path user_path = fs::current_path() / "..\\users" / std::to_string(accnum);
#else
    fs::path user_path = fs::current_path() / "../users" / std::to_string(accnum);
#endif

    if (!fs::exists(user_path) || !fs::is_directory(user_path))
    {
        _status = false;
        return false;
    }

    fs::path metadata_file = user_path / "Metadata.bin";

    // Open the file in binary mode
    std::ifstream metadata(metadata_file, std::ios::binary);
    if (!metadata.is_open())
    {
        _status = false;
        return false;
    }

    // Read the entire file into the buffer
    metadata.seekg(0, std::ios::end);
    std::streamsize size = metadata.tellg();
    metadata.seekg(0, std::ios::beg);

    buf->resize(size); // Resize the buffer to match the file size
    if (!metadata.read(buf->data(), size))
    {
        _status = false;
        return false;
    }

    metadata.close();
    return true;
}

bool fetch::fetch_user_transaction(accnum_t accnum, std::string *buf)
{
#ifdef _WIN32
    fs::path user_path = fs::current_path() / "..\\users" / std::to_string(accnum);
#else
    fs::path user_path = fs::current_path() / "../users" / std::to_string(accnum);
#endif

    if (!fs::exists(user_path) || !fs::is_directory(user_path))
    {
        _status = false;
        return false;
    }

    fs::path transactions_file = user_path / "Transactions.bin";

    std::ifstream metadata(transactions_file, std::ios::binary);
    if (!metadata.is_open())
    {
        _status = false;
        return false;
    }

    metadata.seekg(0, std::ios::end);
    std::streamsize size = metadata.tellg();
    metadata.seekg(0, std::ios::beg);

    buf->resize(size);
    if (!metadata.read(buf->data(), size))
    {
        _status = false;
        return false;
    }

    metadata.close();
    return true;
}

accounts::User fetch::fetch_user(std::string metadata, std::string transaction, accnum_t acc)
{
    _status = true;
    accounts::User user;

    uint8_t account_type_byte;
    double balance = 0;
    size_t pin = 0;
    size_t username_length = 0;
    std::string user_name;
    std::time_t creation_date = 0;
    accounts::__account_t account_type;
    double tlim = 0;
    std::istringstream metadata_stream(metadata, std::ios::binary);

    metadata_stream.read(reinterpret_cast<char *>(&account_type_byte), sizeof(account_type_byte));
    account_type = static_cast<accounts::__account_t>(account_type_byte);

    metadata_stream.read(reinterpret_cast<char *>(&balance), sizeof(balance));
    metadata_stream.read(reinterpret_cast<char *>(&pin), sizeof(pin));

    metadata_stream.read(reinterpret_cast<char *>(&tlim), sizeof(double));

    metadata_stream.read(reinterpret_cast<char *>(&username_length), sizeof(username_length));

    char name[username_length + 1];
    metadata_stream.read(name, username_length);
    name[username_length] = 0;
    user_name = name;

    metadata_stream.read(reinterpret_cast<char *>(&creation_date), sizeof(creation_date));

    std::vector<accounts::Transaction> transactions_list;

    if (!(transaction.length() == 1))
    {
        std::istringstream transactions_stream(transaction, std::ios::binary);

        while (transactions_stream.peek() != EOF)
        {
            accounts::Transaction trans;

            transactions_stream.read(reinterpret_cast<char *>(&trans.__sender), sizeof(accnum_t));
            transactions_stream.read(reinterpret_cast<char *>(&trans.__receiver), sizeof(accnum_t));
            transactions_stream.read(reinterpret_cast<char *>(&trans.__transaction_amount), sizeof(double));

            uint8_t type_byte;
            transactions_stream.read(reinterpret_cast<char *>(&type_byte), sizeof(type_byte));
            trans.type = static_cast<accounts::__transaction_t>(type_byte);

            transactions_stream.read(reinterpret_cast<char *>(&trans.__time_of_transaction), sizeof(std::time_t));

            transactions_list.push_back(trans);
        }
    }
    user = accounts::User(transactions_list, tlim, pin, user_name, acc, balance, account_type, creation_date);

    return user;
}