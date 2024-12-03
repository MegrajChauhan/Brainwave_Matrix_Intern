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
    if (amount > transaction_limit || amount > current_balance)
        return false;
    Transaction _t;
    _t.type = __WITHDRAWING;
    _t.__transaction_amount = amount;
    all_transaction.push_back(_t);
    current_balance -= amount;
    return true;
}

bool accounts::User::deposit(double amount)
{
    if (amount > transaction_limit)
        return false;
    Transaction _t;
    _t.type = __DEPOSITING;
    _t.__transaction_amount = amount;
    all_transaction.push_back(_t);
    current_balance -= amount;
    return true;
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
        other_end.add_transaction(_t);
        _t.type = __RECEIVING;
        all_transaction.push_back(_t);
    }
}

std::vector<accounts::Transaction> accounts::User::get_transaction_history()
{
    return all_transaction;
}