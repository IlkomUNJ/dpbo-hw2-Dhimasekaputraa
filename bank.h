#ifndef BANK_H
#define BANK_H

#include "bank_customer.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Bank {
private:
    string name;
    vector<BankCustomer> accounts;
    int nextId;

public:
    Bank(const string& name) {
        this->name = name;
        this->nextId = 1;
    }

    // Create new account
    BankCustomer createAccount(const string& customerName, double initialDeposit) {
        BankCustomer newCustomer(nextId, customerName, initialDeposit);
        accounts.push_back(newCustomer);
        cout << "Bank account created successfully! Account ID: " << nextId << endl;
        nextId++;
        return newCustomer;
    }

    // Find account by ID
    BankCustomer* findAccountById(int id) {
        for (auto &acc : accounts) {
            if (acc.getId() == id) {
                return &acc;
            }
        }
        return nullptr;
    }

    // Display all accounts
    void displayAllAccounts() const {
        cout << "\n=== BANK ACCOUNTS LIST ===\n";
        if (accounts.empty()) {
            cout << "No accounts found.\n";
        } else {
            for (const auto &acc : accounts) {
                cout << "ID: " << acc.getId()
                     << " | Name: " << acc.getName()
                     << " | Balance: $" << acc.getBalance() << endl;
            }
        }
    }

    // Deposit
    void depositToAccount(int id, double amount) {
        BankCustomer* acc = findAccountById(id);
        if (acc) {
            acc->addBalance(amount);
            cout << "Deposit successful! New balance: $" << acc->getBalance() << endl;
        } else {
            cout << "Account not found.\n";
        }
    }

    // Withdraw
    void withdrawFromAccount(int id, double amount) {
        BankCustomer* acc = findAccountById(id);
        if (acc) {
            if (acc->withdrawBalance(amount)) {
                cout << "Withdrawal successful! Remaining balance: $" << acc->getBalance() << endl;
            }
        } else {
            cout << "Account not found.\n";
        }
    }

    // Delete Account
    void removeAccount(int id) {
        auto initialSize = accounts.size();
        accounts.erase(std::remove_if(accounts.begin(), accounts.end(),
                                     [id](const BankCustomer& acc) {
                                         return acc.getId() == id;
                                     }),
                       accounts.end());

        if (accounts.size() < initialSize) {
            cout << "Bank account ID " << id << " deleted.\n";
        }
    }
};

#endif // BANK_Hz