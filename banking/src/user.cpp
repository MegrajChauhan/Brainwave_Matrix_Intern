#include "user.hpp"
#include "fetch.hpp"

bool accounts::User::log_in(size_t pin)
{
    if (user_pin == pin)
        logged_on = true;
    else
        logged_on = false;
    return logged_on;
}

void accounts::User::log_out()
{
    logged_on = false;
}

bool accounts::User::is_logged_in()
{
    return logged_on;
}

double accounts::User::get_balance()
{
    return current_balance;
}

bool accounts::User::withdraw(double amount)
{
    if (amount > transaction_limit)
    {
        temp_msg = "Transaction Limit crossed";
        return false;
    }
    if (amount > current_balance)
    {
        temp_msg = "Insufficient Balance";
        return false;
    }
    Transaction _t;
    _t.type = __WITHDRAWING;
    _t.__transaction_amount = amount;
    _t.__time_of_transaction = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    all_transaction.push_back(_t);
    current_balance -= amount;
    return true;
}

bool accounts::User::deposit(double amount)
{
    if (amount > transaction_limit)
    {
        temp_msg = "Transaction Limit crossed";
        return false;
    }
    Transaction _t;
    _t.type = __DEPOSITING;
    _t.__transaction_amount = amount;
    _t.__time_of_transaction = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    all_transaction.push_back(_t);
    current_balance += amount;
    return true;
}

double accounts::User::get_lim()
{
    return transaction_limit;
}

void accounts::User::add_transaction(Transaction transaction)
{
    all_transaction.push_back(transaction);
}

void accounts::User::add_transaction(double amount, bool sending, accnum_t other)
{
    accounts::User other_end = fetch::fetch_user(other);
    if (!fetch::fetch_status())
    {
        // In a real-world application here, this would do a lot more under the hood
        // than simply flipping a value
        log_out(); // log out and convey error(using logged_on)
        return;
    }
    // We need to put a transaction to the receiving end too
    Transaction _t;
    _t.__time_of_transaction = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    if (sending)
    {
        _t.__receiver = other;
        _t.__sender = account_number;
        _t.type = __RECEIVING;
        _t.__transaction_amount = amount;
        _t.__time_of_transaction = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        other_end.add_transaction(_t);
        _t.type = __SENDING;
        all_transaction.push_back(_t);
    }
    else
    {
        _t.__receiver = account_number;
        _t.__sender = other;
        _t.type = __SENDING;
        _t.__transaction_amount = amount;
        _t.__time_of_transaction = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        other_end.add_transaction(_t);
        _t.type = __RECEIVING;
        all_transaction.push_back(_t);
    }
}

void accounts::User::add_transaction(bool sending, double amount, accounts::User &other)
{
    // We need to put a transaction to the receiving end too
    Transaction _t;
    _t.__time_of_transaction = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    if (sending)
    {
        _t.__receiver = other.get_accnum();
        _t.__sender = account_number;
        _t.type = __RECEIVING;
        _t.__transaction_amount = amount;
        _t.__time_of_transaction = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        other.add_transaction(_t);
        _t.type = __SENDING;
        all_transaction.push_back(_t);
    }
    else
    {
        _t.__receiver = account_number;
        _t.__sender = other.get_accnum();
        _t.type = __SENDING;
        _t.__time_of_transaction = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        _t.__transaction_amount = amount;
        other.add_transaction(_t);
        _t.type = __RECEIVING;
        all_transaction.push_back(_t);
    }
}

bool accounts::User::transfer(accnum_t receiver, double amount)
{
    if (receiver == account_number)
    {
        temp_msg = "Cannot transfer money to self";
        return false;
    }
    accounts::User other_end = fetch::fetch_user(receiver);
    if (!fetch::fetch_status())
    {
        return false;
    }
    if (amount > transaction_limit)
    {
        temp_msg = "Transaction Limit crossed";
        return false;
    }
    if (amount > current_balance)
    {
        temp_msg = "Insufficient Balance";
        return false;
    }
    Transaction _t;
    _t.type = __SENDING;
    _t.__receiver = receiver;
    _t.__transaction_amount = amount;
    _t.__time_of_transaction = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    all_transaction.push_back(_t);
    current_balance -= amount;

    _t.type = __RECEIVING;
    _t.__transaction_amount = amount;
    _t.__sender = account_number;
    other_end.add_transaction(_t);
    other_end.current_balance += amount;
    fetch::unfetch_user(other_end);

    return true;
}

std::vector<accounts::Transaction> accounts::User::get_transaction_history()
{
    return all_transaction;
}

accnum_t accounts::User::get_accnum() { return account_number; }

std::time_t accounts::User::get_doc() { return date_of_creation; }

accounts::__account_t accounts::User::get_account_type() { return type; }

std::string accounts::User::get_username() { return user_name; }

bool accounts::User::write_back()
{
    return fetch::unfetch_user(*this);
}

size_t accounts::User::get_pin()
{
    return user_pin;
}

std::string accounts::User::get_tmp_msg()
{
    return temp_msg;
}