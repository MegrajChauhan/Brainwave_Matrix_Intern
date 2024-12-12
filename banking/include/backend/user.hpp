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
        // we are not going to support a Joint account yet
    };

    enum __transaction_t
    {
        // Instead of the accouting terms "Credit" and "Debit"
        __SENDING,
        __RECEIVING,
        __WITHDRAWING,
        __DEPOSITING,
    };

    struct Transaction
    {
        accnum_t __receiver;
        accnum_t __sender;
        double __transaction_amount;
        std::time_t __time_of_transaction;
        __transaction_t type;

        Transaction(accnum_t anum, accnum_t snum, double traam, std::time_t tot, __transaction_t t) : __receiver(anum), __sender(snum), __transaction_amount(traam), __time_of_transaction(tot), type(t) {}

        Transaction() = default;
    };

    class User
    {
        std::vector<Transaction> all_transaction;
        size_t transaction_limit;
        size_t user_pin;
        std::string user_name;
        accnum_t account_number;
        double current_balance;
        __account_t type;
        std::time_t date_of_creation;
        bool logged_on = false;
        std::string temp_msg;

    public:
        User(
            std::vector<Transaction> transactions, size_t _t_limit, size_t _pin, std::string name,
            accnum_t accnum, double balance, __account_t _type, std::time_t _doc) : all_transaction(transactions), transaction_limit(_t_limit), user_pin(_pin), user_name(name),
                                                                                    account_number(accnum), current_balance(balance), type(_type), date_of_creation(_doc) {}
        User() = default;

        /**
         * The log in and log out functions might seems simple but the real implementation
         * would be rather complex. For eg: Logging in wouldn't be as simple as just setting
         * a flag. The request must be forwarded to the Bank's databases. Various steps need
         * to be taken in-between which is why this is a "simple" simulation
         */

        void add_transaction(double amount, bool sending, accnum_t other);

        void add_transaction(Transaction transaction);

        void add_transaction( bool sending, double amount, User &other);

        std::time_t get_doc();

        __account_t get_account_type();

        accnum_t get_accnum();

        bool write_back(); // save the changes

        std::string get_username();

        bool log_in(size_t pin);

        bool is_logged_in();

        size_t get_pin(); // not safe

        double get_balance();

        bool withdraw(double amount);

        bool deposit(double amount);

        double get_lim();

        bool transfer(accnum_t receiver, double amount);

        std::string get_tmp_msg();

        std::vector<Transaction> get_transaction_history();

        void log_out();
    };
};

#endif
