#ifndef _SESSION_
#define _SESSION_

/**
 * Session will control the top body of the screen
 * while IO will control first_line
 */

#include "console.hpp"
#include "API.hpp"
#include "IO.hpp"

namespace atm
{
    // Any errors causes immediate termination.
    // We have no safety in-place for logged in accounts in case of a crash
    // We need to make sure that the when the ATM crashes, it crashes safely
    // without any unnecessary corruptions, bugs, and exploits.
    static Console *c;

    void preinit();

    void start_atm();

    bool verify_account_num(std::string accnum);
    
    bool atm_login();

    bool atm_logout();

    void atm_help();

};

#endif