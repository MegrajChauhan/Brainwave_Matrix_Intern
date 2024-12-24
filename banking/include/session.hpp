#ifndef _SESSION_
#define _SESSION_

/**
 * Session will control the top body of the screen
 * while IO will control first_line
 */

#include "console.hpp"
#include "API.hpp"
#include "IO.hpp"
#include "client.hpp"
#include <random>
#include <sstream>
#include <algorithm>
#include <chrono>

namespace bank
{
    // Any errors causes immediate termination.
    // We have no safety in-place for logged in accounts in case of a crash
    // We need to make sure that the when the bank crashes, it crashes safely
    // without any unnecessary corruptions, bugs, and exploits.
    static Console *c;

    void preinit();

    void start_bank();

    bool verify_account_num(std::string accnum);
    
    bool bank_login();

    bool bank_logout();

    void bank_help();

    void bank_register_user();

    void bank_print_user_details();

    void bank_withdraw();

    void bank_deposit();

    void bank_transfer();

};

#endif