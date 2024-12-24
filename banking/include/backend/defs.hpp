#ifndef _DEFS_
#define _DEFS_

typedef unsigned long accnum_t;

enum serverReply_t
{
    _CONNECTION_MADE,
    _ALREADY_LOGIN,
    _NOT_LOGGED_ON,
    _INCORRECT_PIN,
    _USER_EXISTS,
    _SUCCESS,
    _ERROR,
};

enum clientMessage_t
{
    _CLOSE_CONN,
    _GET_METADATA, // fetch details about a user
    _GET_TRANSACTION, // fetch transaction details about a user
    _LOGIN,
    _LOGOUT,
    _REGISTER,
    _DEPOSIT,
    _TRANSFER,
    _WITHDRAW,
};

#endif