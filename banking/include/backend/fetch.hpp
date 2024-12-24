#ifndef _FETCH_
#define _FETCH_

/**
 * This will simulate getting the user information based on the account number.
 * We will be inefficient here since this is going to be a basic simulation and fetch
 * everything at once.
 * A typical bank account with enough age should have thousands of transactions.
 * Fetching all of them at once is rather slow when withdrawing cash is more important than
 * knowing the history. Thus, we will, for the sake of this simulation, fetch everything
 * about a user and only move forward afterwards.
 */

/**
 * As we don't have a database to make use of, the approach is going to be very simple.
 * Once again, with only the topic as the information, i cannot deduce if the use of a
 * database is mandatory and thus, directories will be used.
 * 
 * Here is the basic structure of a single user:
 * <Account Number>
 *    <Metadata>
 *    <Transactions>
 *       <1>
 *       <2>
 *       .
 *       .
 *       . 
 * Using Transaction IDs would have made things simpler in a database env but for us, simple
 * numbers will suffice.
 */

#include <filesystem>
#include <string>
#include "user.hpp"

namespace fs = std::filesystem;

namespace fetch
{
    /**
     * This module expects that the user files are not meddled with, 
     * deleted or duplicated.
     */

    // For simplicity sake(complexity already high due to the use of RAW mode),
    // we will use variables like these to convey status
    static bool _status = true;

    bool user_exists(accnum_t accnum);

    accounts::User fetch_user(accnum_t accum);

    bool unfetch_user(accounts::User *user);

    bool fetch_status();

    accounts::User fetch_user(std::string metadata, std::string transaction, accnum_t acc);

    bool create_user(accnum_t accnum, std::string name, size_t pin, accounts::__account_t type);

    bool fetch_user_metadata(accnum_t accnum, std::string *buf);

    bool fetch_user_transaction(accnum_t accnum, std::string *buf);
};

#endif