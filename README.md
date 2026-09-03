# 🏦 Banking System with ATM Simulation

A C++ console-based banking and ATM simulation built to demonstrate **Object-Oriented Programming (OOP)**, account management, transaction processing, exception handling, file handling, and polymorphism.

## 📌 About the Project

This project simulates a basic ATM banking system where a user can log in using a PIN and perform common banking operations such as checking their balance, depositing money, withdrawing money, and viewing transaction history.

The system includes support for multiple account types, including savings and current accounts, while the current demo runs with a single savings account. See [Known Limitations](#-known-limitations).

## ✨ Features

* 🔐 PIN-based ATM authentication
* 💰 Balance inquiry
* 💵 Deposit money
* 💸 Withdraw money
* 📋 Transaction history / mini-statement
* 💾 Save account data to text files
* 📂 Load previously saved account data
* 🏦 `SavingAccount` class with interest-calculation logic (`applyInterest()`)
* 🏦 `CurrentAccount` class with overdraft-limit logic
* ⚠️ Input and transaction validation for deposit/withdraw amounts
* ❌ Exception handling for invalid operations
* 🔄 Operator overloading for account operations
* 📦 Template-based account manager (`BankManager<T>`)

> **Note:** `applyInterest()` and `CurrentAccount` are fully implemented but not currently exposed through the ATM menu or `main()`. See [Known Limitations](#-known-limitations) for details.

## 🧠 OOP Concepts Demonstrated

### Encapsulation

Account information such as the account holder, account number, balance, and PIN is protected inside the `Account` class.

### Inheritance

`SavingAccount` and `CurrentAccount` inherit from the base `Account` class.

```cpp
class SavingAccount : public Account
```

```cpp
class CurrentAccount : public Account
```

### Polymorphism

The `withdraw()` function is declared as a pure virtual function in the `Account` class and implemented differently by each account type.

```cpp
virtual void withdraw(double amount) = 0;
```

### Operator Overloading

The project overloads:

* `==` → compares account balances
* `+=` → shorthand deposit
* `-=` → shorthand deduction

### Abstraction

The `Account` class provides a common interface for different account types while leaving withdrawal behavior to the derived classes.

## 🛠️ Technologies Used

* **C++**
* Object-Oriented Programming
* File Handling
* Exception Handling
* STL `vector`
* C++ Templates
* `fstream`
* `ctime`

## 🏗️ Main Classes

| Class            | Purpose                                            | Used in current demo?            |
| ---------------- | -------------------------------------------------- | -------------------------------- |
| `Transaction`    | Stores and displays transaction information        | ✅                                |
| `Account`        | Base class containing common account functionality | ✅                                |
| `SavingAccount`  | Handles savings accounts and interest              | ✅                                |
| `CurrentAccount` | Handles current accounts and overdraft             | ⚠️ Implemented, not instantiated |
| `ATM`            | Handles PIN authentication and ATM operations      | ✅                                |
| `BankManager<T>` | Manages account objects using a template           | ⚠️ Only one account currently    |

## 🔄 How It Works

```text
Start Program
     ↓
Create / Load Account
     ↓
Enter ATM PIN
     ↓
PIN Verification
     ↓
ATM Menu
     ↓
┌─────────────────────┐
│ 1. Check Balance    │
│ 2. Deposit          │
│ 3. Withdraw         │
│ 4. Mini-Statement   │
│ 5. Exit             │
└─────────────────────┘
     ↓
Save Account Data
     ↓
Exit
```

## 💾 Data Persistence

The application uses text files to save account information and transaction history.

For example:

```text
SA-5543.txt
```

Account information is saved when the user exits the ATM, allowing the program to load the saved account data the next time it runs.

Persistence is intentionally simple (comma-separated fields, no escaping), which works for the current use case but isn't a robust storage format — see limitations below.

## ⚠️ Exception Handling

The project uses C++ `runtime_error` exceptions to handle situations such as:

* Invalid PIN
* Invalid deposit amount
* Invalid withdrawal amount
* Insufficient funds
* Exceeding the overdraft limit

Example:

```cpp
throw runtime_error("Invalid Withdrawal Amount Exception!");
```

## 🚧 Known Limitations

Being upfront about the current state of the project:

* **`main()` only creates a `SavingAccount`.** `CurrentAccount` is fully implemented and functional but never instantiated or exercised in the live demo.

* **`applyInterest()` is not reachable from the ATM menu.** The logic exists on `SavingAccount` but there's no menu option that calls it, so interest is never actually applied during a run.

* **`BankManager<T>` currently manages a single account.** The template is built to hold a `vector<T*>`, but only one account is ever added, so its multi-account capability isn't demonstrated yet.

* **No malformed-input handling for menu choices/amounts.** Entering non-numeric input at a `cin >>` prompt (e.g. typing letters when an amount is expected) isn't caught by the `runtime_error` handling and can leave `cin` in a fail state rather than being gracefully rejected.

* **File persistence format is minimal.** Transaction data is stored as comma-separated values with no escaping, which is fine for the current fixed `type` values (`Deposit`/`Withdrawal`) but wouldn't scale to richer data without a proper serialization format.

## ▶️ How to Run

### 1. Clone the repository

```bash
git clone https://github.com/Mah-noor5/Banking-Simulation-CPP.git
```

### 2. Open the project

Open the project folder in **VS Code**, **Dev-C++**, or another C++ IDE.

### 3. Compile

Using g++:

```bash
g++ ATM.cpp -o ATM
```

### 4. Run

Windows:

```bash
ATM.exe
```

## 🔑 Default Test Account

The current program starts with:

```text
Account Holder: Zain Ahmed
Account Number: SA-5543
Initial Balance: $2000
PIN: 1234
```

**Note:** This is a demonstration project. The PIN and account information are hard-coded for testing purposes and should not be used for a real banking application.

## 📚 What I Learned

Through this project, I practiced:

* Designing classes and objects
* Encapsulation and abstraction
* Inheritance and polymorphism
* Virtual and pure virtual functions
* Operator overloading
* C++ templates
* Exception handling
* File input/output
* STL vectors
* Transaction management
* Structuring a larger C++ console application

## 🚀 Future Improvements

Possible improvements include:

* Wire `applyInterest()` and `CurrentAccount` into the ATM menu/demo
* Support for multiple accounts through `BankManager<T>`
* Account creation through the ATM
* Secure password/PIN storage
* Improved input validation (reject non-numeric console input gracefully)
* Database integration
* Transfer between accounts
* GUI interface
* More detailed transaction records
* Separate header and source files

## 👨‍💻 Author

**Mah-Noor Jamil**

GitHub: [Mah-noor5](https://github.com/Mah-noor5)
