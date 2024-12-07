#ifndef _API_
#define _API_

/**
 * The main purpose of this backend is to allow it to be used somewhere else if needed.
 * The API is what the frontend will use while the implementation may access some database
 * to obtain the required data.
 */

#include <vector>
#include <string>
#include "user.hpp"
#include "fetch.hpp"

namespace API
{
    // The API will also act as a context and hold the current session
    static accounts::User current_session;

    bool log_in(accnum_t accnum, size_t pin);
    
    bool log_out();

    double get_balance();

    std::string get_username();

    std::vector<accounts::Transaction> get_transaction_history();

    accnum_t get_accnum();

    std::time_t get_doc();

    accounts::__account_t get_account_type();

    bool withdraw(double amount);

    bool deposit(double amount);

    bool transfer(accnum_t target_accnum, double amount);

    bool session_active();

    std::string account_type_to_string(accounts::__account_t type);

    std::string transaction_type_to_string(accounts::__transaction_t type);

    std::string get_tmp_msg();

};

#endif
