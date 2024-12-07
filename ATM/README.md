# ATM Simulation Project

This is an ATM simulation program developed as a task for an internship. The project provides a basic simulation of ATM functionalities, allowing users to perform common banking operations like deposits, withdrawals, and transfers. The program also supports user account management, including login/logout and balance inquiries.

---

## Features

1. **User Authentication**
   - Log in securely using an account number and PIN.
   - Log out to end the session.

2. **Account Management**
   - View account details, including balance and transaction history.

3. **Banking Operations**
   - Deposit funds into your account.
   - Withdraw funds from your account.
   - Transfer funds to other accounts.
   - Check your current balance.

4. **Command Interface**
   - Supports a wide range of commands for seamless interaction.
   - Includes a help menu for user guidance.

5. **Error Handling**
   - Invalid commands are gracefully handled with appropriate error messages.

---

## Prerequisites

Ensure the following software is installed on your system:
- **C++ Compiler**: Compatible with `g++`.
- **Make**: For build automation.

---

## Building the Project

The project can be built in two modes:
1. **Debug Mode**: Includes debugging symbols for development.
2. **Build Mode**: Optimized for release.

### Build Command

To build the project, run:

```bash
make all mode=<build/debug>
```
Replace `<build/debug>` with build for release mode or debug for debugging mode.

# Running the Project

After building the executable, run:
```bash
./build/atm
```

# Creating Users

To create new users in the system, use the create_user command:
```bash
./build/create_user
```
This will prompt you to enter some details following which a new user will be added to the **users/** folder.

# LICENSE

This project is proprietary and developed exclusively for the internship program.
