#include "API.hpp"

bool API::log_in(accnum_t accnum, size_t pin)
{
    bank::msg m;
    m.as_long1 = 0, m.as_long2 = 0;
    m.as_long1 = _LOGIN;
    m.as_long1 <<= 56;
    m.as_long1 |= (pin & 0xFFFF);
    m.as_long2 = accnum;
    bank::send_msg(m);
    std::string reply = bank::get_reply();
    if (reply[0] != (char)_SUCCESS)
    {
        handle_failure((serverReply_t)reply[0]);
        return false;
    }

    m.as_long1 = _GET_METADATA;
    m.as_long1 <<= 56;
    bank::send_msg(m);
    std::string metadata = bank::get_reply();

    m.as_long1 = _GET_TRANSACTION;
    m.as_long1 <<= 56;
    bank::send_msg(m);
    std::string transaction = bank::get_reply();

    accounts::User user = fetch::fetch_user(metadata, transaction, accnum);
    if (!fetch::fetch_status())
        return false;
    user.log_in(pin);
    current_session = user;
    if(metadata.length() < 1){
        err_msg="no metadata";
        return false;
    }
    return true;
}

bool API::log_out()
{
    bank::msg m;
    m.as_long1 = 0, m.as_long2 = 0;
    if (current_session.is_logged_in())
    {
        m.as_bytes[7] = _LOGOUT;
        m.as_long2 = current_session.get_accnum();
        bank::send_msg(m);
        std::string tmp = bank::get_reply();
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
        bank::msg m;
        m.as_long1 = 0, m.as_long2 = 0;
        m.as_bytes[7] = _WITHDRAW;
        m.as_long2 = current_session.get_accnum();
        bank::send_msg(m);
        m.as_long1 = amount;
        bank::send_msg(m);
        std::string temp = bank::get_reply();
        if (temp[0] != (char)_SUCCESS)
        {
            handle_failure((serverReply_t)temp[0]);
            return false;
        }
        return current_session.withdraw(amount);
    }
    return false;
}

bool API::deposit(double amount)
{
    if (current_session.is_logged_in())
    {
        bank::msg m;
        m.as_bytes[7] = _DEPOSIT;
        m.as_long2 = current_session.get_accnum();
        bank::send_msg(m);
        m.as_long1 = amount;
        bank::send_msg(m);
        std::string temp = bank::get_reply();
        if (temp[0] != (char)_SUCCESS)
        {
            handle_failure((serverReply_t)temp[0]);
            return false;
        }
        return current_session.deposit(amount);
    }
    return false;
}

bool API::transfer(accnum_t target_accnum, double amount)
{
    if (current_session.is_logged_in())
    {
        bank::msg m;
        m.as_bytes[7] = _TRANSFER;
        m.as_long2 = current_session.get_accnum();
        bank::send_msg(m);
        m.as_long1 = target_accnum;
        m.as_long2 = amount;
        bank::send_msg(m);
        std::string temp = bank::get_reply();
        if (temp[0] != (char)_SUCCESS)
        {
            handle_failure((serverReply_t)temp[0]);
            return false;
        }
        return current_session.transfer_simple(target_accnum, amount);
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

std::string API::transaction_type_to_string(accounts::__transaction_t type)
{
    switch (type)
    {
    case accounts::__transaction_t::__DEPOSITING:
        return "Deposit";
    case accounts::__transaction_t::__RECEIVING:
        return "Received";
    case accounts::__transaction_t::__SENDING:
        return "Sent";
    case accounts::__transaction_t::__WITHDRAWING:
        return "Withdraw";
    }
}

std::string API::get_tmp_msg()
{
    return session_active() ? current_session.get_tmp_msg() : "";
}

bool API::register_user(accnum_t *accnum, std::string name, size_t pin, accounts::__account_t type)
{
    bank::msg m;
    m.as_long1 = 0, m.as_long2 = 0;
    m.as_long1 = _REGISTER;
    m.as_long1 <<= 8;
    m.as_long1 |= type;
    m.as_long1 <<= 32;
    m.as_long1 |= (name.length() & 0xFFFF);
    m.as_long1 <<= 16;
    m.as_long1 |= (pin & 0xFFFF);
    bank::send_msg(m);
    bank::send_raw(name);
    std::string temp = bank::get_reply();
    if (temp[0] != (char)_SUCCESS)
    {
        handle_failure((serverReply_t)temp[0]);
        return false;
    }
    *accnum = std::strtoull(temp.c_str() + 1, nullptr, 10);
    return true;
}

void API::handle_failure(serverReply_t r)
{
    switch (r)
    {
    case serverReply_t::_ALREADY_LOGIN:
        err_msg = "The user is already logged on.";
        break;
    case serverReply_t::_USER_EXISTS:
        err_msg = "The user already exists";
        break;
    case serverReply_t::_INCORRECT_PIN:
        err_msg = "The given PIN is invalid";
        break;
    case serverReply_t::_NOT_LOGGED_ON:
        err_msg = "The user is not logged on to perform this action";
        break;
    case serverReply_t::_ERROR:
        err_msg = "Server encountered error....";
        break;
    }
}

std::string API::get_error_msg()
{
    return err_msg;
}

std::string API::get_error()
{
    return err_msg.empty() ? current_session.get_tmp_msg() : err_msg;
}