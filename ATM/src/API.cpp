#include "API.hpp"

bool API::log_in(accnum_t accnum, size_t pin)
{
    accounts::User user = fetch::fetch_user(accnum);
    if (!fetch::fetch_status())
        return false;
    if (user.log_in(pin))
    {
        current_session = user;
        return true;
    }

    return false;
}

bool API::log_out()
{
    if (current_session.is_logged_in())
    {
        current_session.log_out();
        current_session = accounts::User();
        return true;
    }
    return false;
}

double API::get_balance()
{
    if (current_session.is_logged_in())
    {
        return current_session.get_balance();
    }
    return 0.0;
}

bool API::withdraw(double amount)
{
    if (current_session.is_logged_in())
    {
        return current_session.withdraw(amount);
    }
    return false;
}

bool API::deposit(double amount)
{
    if (current_session.is_logged_in())
    {
        return current_session.deposit(amount);
    }
    return false;
}

bool API::transfer(accnum_t target_accnum, double amount)
{
    if (current_session.is_logged_in())
    {
        accounts::User target_user = fetch::fetch_user(target_accnum);
        if (current_session.withdraw(amount))
        {
            target_user.deposit(amount);
            return true;
        }
    }
    return false;
}

std::vector<accounts::Transaction> API::get_transaction_history()
{
    if (current_session.is_logged_in())
    {
        return current_session.get_transaction_history();
    }
    return std::vector<accounts::Transaction>();
}

bool API::session_active()
{
    return current_session.is_logged_in();
}

std::string API::get_username()
{
    if (current_session.is_logged_in())
        return current_session.get_username();
    return "";
}

accnum_t API::get_accnum()
{
    return current_session.get_accnum();
}

std::time_t API::get_doc()
{
    return current_session.get_doc();
}

accounts::__account_t API::get_account_type()
{
    return current_session.get_account_type();
}

std::string API::account_type_to_string(accounts::__account_t type)
{
    switch (type)
    {
    case accounts::__account_t::__BUSINESS_ACCOUNT:
        return "Business Account";
    case accounts::__account_t::__CHECKING:
        return "Checking Account";
    case accounts::__account_t::__SAVING:
        return "Saving Account";
    }
}