#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <stdexcept>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::ofstream;
using std::ifstream;
using std::runtime_error;


// Stores transaction details
class Transaction {
private:
    string type;
    double amount;
    string timestamp;

public:

    Transaction(string t, double amt) {
        type = t;
        amount = amt;

        // Get current date and time
        time_t now = time(0);
        char* dt = ctime(&now);
        timestamp = string(dt);

        // Remove newline added by ctime()
        if (!timestamp.empty() && timestamp.back() == '\n') {
            timestamp.pop_back();
        }
    }

    // Convert transaction data into a file-friendly format
    string getFileData() const {
        return type + "," + std::to_string(amount) + "," + timestamp;
    }

    // Display transaction information
    void displayTransaction() const {
        cout << "[" << timestamp << "] " << type << ": $" << amount << endl;
    }
};


// Abstract base class for all accounts
class Account {
protected:
    string accountHolder;
    string accountNumber;
    double balance;
    string pin;

    // Stores all account transactions
    vector<Transaction> history;

public:

    Account(string name, string accNum, double initialBalance, string accountPin) {
        accountHolder = name;
        accountNumber = accNum;
        balance = initialBalance;
        pin = accountPin;
    }

    virtual ~Account() {}

    string getAccountNumber() const {
        return accountNumber;
    }

    // Verify ATM PIN
    bool verifyPIN(string inputPin) const {
        if (pin != inputPin) {
            throw runtime_error("Invalid PIN Exception! Access Denied.");
        }
        return true;
    }

    // Deposit money into account
    void deposit(double amount) {
        if (amount <= 0) {
            throw runtime_error("Invalid Deposit Amount Exception!");
        }

        balance += amount;
        history.push_back(Transaction("Deposit", amount));

        cout << "Converted: $" << amount
             << " | New Balance: $" << balance << endl;
    }

    // Pure virtual function (polymorphism)
    virtual void withdraw(double amount) = 0;

    // Compare balances of two accounts
    bool operator==(const Account& other) const {
        return this->balance == other.balance;
    }

    // Operator overloading for deposit
    void operator+=(double amount) {
        if (amount > 0) {
            this->balance += amount;
            cout << "Shorthand Deposit! New Balance: $" << balance << endl;
        }
    }

    // Operator overloading for withdrawal
    void operator-=(double amount) {
        if (amount > 0 && amount <= this->balance) {
            this->balance -= amount;
            cout << "Shorthand Deduction! Remaining: $" << balance << endl;
        } else {
            cout << "Shorthand Deduction Failed." << endl;
        }
    }

    // Save account information to file
    void saveToFile() const {
        ofstream outFile(accountNumber + ".txt");

        if (!outFile) return;

        outFile << accountHolder << "\n"
                << accountNumber << "\n"
                << balance << "\n"
                << pin << "\n";

        outFile << history.size() << "\n";

        for (size_t i = 0; i < history.size(); i++) {
            outFile << history[i].getFileData() << "\n";
        }

        outFile.close();
    }

    // Load account information from file
    void loadFromFile(string accNum) {
        ifstream inFile(accNum + ".txt");

        if (!inFile) return;

        history.clear();

        std::getline(inFile, accountHolder);
        std::getline(inFile, accountNumber);

        inFile >> balance;
        inFile.ignore();

        std::getline(inFile, pin);

        int savedTransactionCount;
        inFile >> savedTransactionCount;
        inFile.ignore();

        for (int i = 0; i < savedTransactionCount; i++) {
            string line;
            std::getline(inFile, line);

            size_t firstComma = line.find(',');
            size_t secondComma = line.find(',', firstComma + 1);

            if (firstComma != string::npos &&
                secondComma != string::npos) {

                string tType = line.substr(0, firstComma);

                double tAmount =
                    std::stod(line.substr(firstComma + 1,
                    secondComma - firstComma - 1));

                history.push_back(Transaction(tType, tAmount));
            }
        }

        inFile.close();
    }

    // Display all transactions
    void displayHistory() const {
        cout << "\n--- History for " << accountNumber << " ---" << endl;

        if (history.empty()) {
            cout << "No transactions recorded." << endl;
            return;
        }

        for (size_t i = 0; i < history.size(); i++) {
            history[i].displayTransaction();
        }
    }

    // Display account details
    void displayDetails() const {
        cout << "\n--- Account Details ---" << endl;
        cout << "Holder: " << accountHolder << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Current Balance: $" << balance << endl;
    }
};


// Savings Account
class SavingAccount : public Account {
private:
    double interestRate;

public:

    SavingAccount(string name,
                  string accNum,
                  double initialBalance,
                  double rate,
                  string pin)
        : Account(name, accNum, initialBalance, pin),
          interestRate(rate) {}

    // Savings account withdrawal
    void withdraw(double amount) override {

        if (amount <= 0) {
            throw runtime_error("Invalid Withdrawal Amount Exception!");
        }

        if (amount > balance) {
            throw runtime_error(
                "Overdraft Exception: Insufficient funds in Saving Account!");
        }

        balance -= amount;
        history.push_back(Transaction("Withdrawal", amount));

        cout << "Converted: $" << amount
             << " | Remaining: $" << balance << endl;
    }

    // Apply interest to balance
    void applyInterest() {
        double interest = balance * interestRate;
        balance += interest;

        cout << "Interest Applied! New Balance: $"
             << balance << endl;
    }
};


// Current Account
class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:

    CurrentAccount(string name,
                   string accNum,
                   double initialBalance,
                   double limit,
                   string pin)
        : Account(name, accNum, initialBalance, pin),
          overdraftLimit(limit) {}

    // Current account withdrawal with overdraft
    void withdraw(double amount) override {

        if (amount <= 0) {
            throw runtime_error("Invalid Withdrawal Amount Exception!");
        }

        if (amount > (balance + overdraftLimit)) {
            throw runtime_error(
                "Overdraft Exception: Exceeded current account overdraft limit!");
        }

        balance -= amount;
        history.push_back(Transaction("Withdrawal", amount));

        cout << "Converted: $" << amount
             << " | Remaining: $" << balance << endl;
    }
};


// ATM class
class ATM {
private:
    Account* currentAccount;

public:

    ATM() : currentAccount(nullptr) {}

    // Login using PIN
    void insertCard(Account* acc) {

        string inputPin;

        cout << "\nEnter ATM PIN: ";
        cin >> inputPin;

        try {
            acc->verifyPIN(inputPin);

            currentAccount = acc;

            cout << "Login Successful!" << endl;
        }
        catch (const runtime_error& error) {

            cout << "ATM ERROR: "
                 << error.what() << endl;

            currentAccount = nullptr;
        }
    }

    // ATM menu operations
    void showMenu() {

        if (currentAccount == nullptr)
            return;

        int choice;
        double amount;

        do {
            cout << "\n===== ATM MENU =====" << endl;
            cout << "1. Check Balance" << endl;
            cout << "2. Deposit" << endl;
            cout << "3. Withdraw" << endl;
            cout << "4. Mini-Statement" << endl;
            cout << "5. Exit ATM" << endl;

            cout << "Enter choice: ";
            cin >> choice;

            try {

                switch (choice) {

                case 1:
                    currentAccount->displayDetails();
                    break;

                case 2:
                    cout << "Enter deposit amount: $";
                    cin >> amount;
                    currentAccount->deposit(amount);
                    break;

                case 3:
                    cout << "Enter withdrawal amount: $";
                    cin >> amount;
                    currentAccount->withdraw(amount);
                    break;

                case 4:
                    currentAccount->displayHistory();
                    break;

                case 5:
                    currentAccount->saveToFile();
                    cout << "Card Ejected. Data Saved." << endl;
                    currentAccount = nullptr;
                    break;

                default:
                    cout << "Invalid choice." << endl;
                }

            }
            catch (const runtime_error& error) {

                cout << "\n*** TRANSACTION FAILED ***" << endl;
                cout << "Reason: " << error.what() << endl;
            }

        } while (choice != 5 &&
                 currentAccount != nullptr);
    }
};


// Template class for managing accounts
template <typename T>
class BankManager {
private:
    vector<T*> accounts;

public:

    ~BankManager() {
        for (auto acc : accounts) {
            delete acc;
        }
    }

    void addAccount(T* acc) {
        accounts.push_back(acc);
    }

    // Save all accounts before program ends
    void saveAllAccounts() {
        for (auto acc : accounts) {
            acc->saveToFile();
        }
    }
};


int main() {

    // Create savings account
    SavingAccount* savings =
        new SavingAccount(
            "Zain Ahmed",
            "SA-5543",
            2000.0,
            0.05,
            "1234"
        );

    // Load previously saved data
    savings->loadFromFile("SA-5543");

    // Add account to manager
    BankManager<Account> manager;
    manager.addAccount(savings);

    // Start ATM system
    ATM machine;
    machine.insertCard(savings);
    machine.showMenu();

    // Save data before exiting
    manager.saveAllAccounts();

    return 0;
}