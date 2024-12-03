#ifndef _USER_
#define _USER_

#include "defs.hpp"
#include <cstddef>
#include <chrono>
#include <vector>
#include <fstream>

/**
 * Despite the name, this actually represents different accounts
 */

/**
 * We are going on with the assumption that the transactions will be made within just one
 * country and so no multiple currency type.
 */

namespace accounts
{
    enum __account_t
    {
        __SAVING,
        __CHECKING,
        __BUSINESS_ACCOUNT,
        // we are not going to support an Joint account yet
    };

    enum __transaction_t
    {
        __SENDING,
        __RECEIVING,
    };

    struct Transaction
    {
        accnum_t __receiver;
        accnum_t __sender;
        size_t __transaction_amount;
        std::time_t __time_of_transaction;
        __transaction_t type;
    };

    class User
    {
        std::vector<Transaction> all_transaction;
        size_t transaction_limit;
        size_t user_pin;
        std::string user_name;
        accnum_t account_number;
        size_t current_balance;
        __account_t type;
        std::time_t date_of_creation;

    public:
        User(
            std::vector<Transaction> transactions, size_t _t_limit, size_t _pin, std::string name,
            accnum_t accnum, size_t balance, __account_t _type, std::time_t _doc) : all_transaction(transactions), transaction_limit(_t_limit), user_pin(_pin), user_name(name),
                                                                                    account_number(accnum), current_balance(balance), type(_type), date_of_creation(_doc) {}
        User() = default;
    };
};

#endif
