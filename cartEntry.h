#ifndef CARTENTRY_H
#define CARTENTRY_H

#include <string>
#include <iostream>
#include "item.h"

using namespace std;

class CartEntry {
private:
    int itemId;
    string itemName;
    double price;
    int quantity;
    int sellerId;

public:
    CartEntry() {}
    CartEntry(int itemId, string itemName, double price, int quantity, int sellerId)
        : itemId(itemId), itemName(itemName), price(price), quantity(quantity), sellerId(sellerId) {}

    int getItemId() const { return itemId; }
    string getItemName() const { return itemName; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    int getSellerId() const { return sellerId; }

    double getTotalPrice() const { return price * quantity; }

    void setQuantity(int q) { quantity = q; }

    void print() const {
        cout << "   Item ID: " << itemId << "\n"
             << "   Name: " << itemName << "\n"
             << "   Price: $" << price << "\n"
             << "   Quantity: " << quantity << "\n"
             << "   Total: $" << getTotalPrice() << endl;
    }

    void increaseQuantity(int amount = 1) { quantity += amount; }
    void decreaseQuantity(int amount = 1) {
        if (quantity > amount)
            quantity -= amount;
        else
            quantity = 0;
    }
};

#endif
