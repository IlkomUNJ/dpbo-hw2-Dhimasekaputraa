#ifndef BUYER_H
#define BUYER_H

#include <string>
#include <iostream>
#include "bank_customer.h" 

using namespace std;

class Buyer {
private:
    int id;
    string name;
    string address;
    string phone;
    string email;
    bool bankAccountCreated = false;
    BankCustomer account = BankCustomer(-1, "", 0.0);

public:
    Buyer(int id, const string& name, const string& address, const string& phone, const string& email, const BankCustomer& account0)
        : id(id), name(name), address(address), phone(phone), email(email), account(account0) {}

    const string& getAddress() const { return address; }
    const string& getPhone() const { return phone; }
    const string& getEmail() const { return email; }
    int getId() const { return id; }
    const string& getName() const { return name; }
    const BankCustomer& getAccount() const { return account; }

    void setId(int newId) { id = newId; }
    void setName(const std::string& newName) { name = newName; }
    void setAddress(const std::string& newAddress) { address = newAddress; }
    void setPhone(const std::string& newPhone) { phone = newPhone; }
    void setEmail(const std::string& newEmail) { email = newEmail; }

    public:
    bool hasBankAccount() const { 
        return bankAccountCreated; 
    }

    void setAccount(const BankCustomer& acc) {
        account = acc;
        bankAccountCreated = true;
    }

    BankCustomer& getAccount() { 
        return account; 
    }
  
    void printInfo() const {
        std::cout << "Buyer ID: " << id << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Address: " << address << std::endl;
        std::cout << "Phone: " << phone << std::endl;
        std::cout << "Email: " << email << std::endl;
    }
};

#endif // BUYER_H
