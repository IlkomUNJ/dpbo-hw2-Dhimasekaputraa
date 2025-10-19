#pragma once
#include "buyer.h"
#include "item.h"
#include "invoice.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

class seller : public Buyer {
private:
    int sellerId;
    std::string storeName;
    std::string storeAddress;
    std::string storePhone;
    std::string storeEmail;
    double deposit;
    std::vector<Item> items;

    bool idDisplayed(int itemId) const {
        return itemId > 0;
    }

public:
    seller() = default;

    seller(Buyer buyer, int sellerId,
           const std::string& storeName,
           const std::string& storeAddress,
           const std::string& storePhone,
           const std::string& storeEmail,
           double deposit)
        : Buyer(buyer.getId(), buyer.getName(), buyer.getAddress(), buyer.getPhone(), buyer.getEmail(), buyer.getAccount()),
          sellerId(sellerId),
          storeName(storeName),
          storeAddress(storeAddress),
          storePhone(storePhone),
          storeEmail(storeEmail),
          deposit(deposit) {}

    virtual ~seller() = default;

    int getSellerId() const { return sellerId; }
    std::string getStoreName() const { return storeName; }
    std::string getStoreAddress() const { return storeAddress; }
    std::string getStorePhone() const { return storePhone; }
    std::string getStoreEmail() const { return storeEmail; }
    double getDeposit() const { return deposit; }

    void addNewItem(int newId, const std::string& newName, int newQuantity, double newPrice) {
        Item newItem(newId, newName, newQuantity, newPrice);
        items.push_back(newItem);
    }

    void updateItem(int itemId, const std::string& newName, int newQuantity, double newPrice) {
        for (auto& item : items) {
            if (item.getId() == itemId) {
                item.alterItemById(itemId, newName, newQuantity, newPrice);
            }
        }
    }

    void makeItemVisibleToCustomer(int itemId) {
        for (auto& item : items) {
            if (item.getId() == itemId) {
                item.setDisplay(true);
                break;
            }
        }
    }

    void displayItems() const {
        if (items.empty()) {
            std::cout << "No items in inventory.\n";
        } else {
            for (const auto& item : items) {
                std::cout << "ID: " << item.getId()
                          << " | Name: " << item.getName()
                          << " | Quantity: " << item.getQuantity()
                          << " | Price: $" << item.getPrice() << "\n";
            }
        }
    }

    const std::vector<Item>& getItems() const { return items; }

    Item* getItemById(int itemId) {
        for (auto &it : items) {
            if (it.getId() == itemId) return &it;
        }
        return nullptr;
    }

    const Item* getItemByIdConst(int itemId) const {
        for (const auto &it : items) {
            if (it.getId() == itemId) return &it;
        }
        return nullptr;
    }

    bool reduceStock(int itemId, int qty) {
        Item* it = getItemById(itemId);
        if (!it) return false;
        if (qty <= 0) return false;
        if (it->getQuantity() < qty) return false;
        it->setQuantity(it->getQuantity() - qty);
        return true;
    }

    bool increaseStock(int itemId, int qty) {
        Item* it = getItemById(itemId);
        if (!it) return false;
        if (qty <= 0) return false;
        it->setQuantity(it->getQuantity() + qty);
        return true;
    }

    void removeItemFromInventory(int itemId){
        auto initialSize = items.size();
        items.erase(std::remove_if(items.begin(), items.end(),
                    [itemId](const Item& item){
                        return item.getId() == itemId;
                    }),
                items.end());
            if (items.size() < initialSize) {
            std::cout << "Item with ID " << itemId << " removed successfully.\n";
        } else {
            std::cout << "Item with ID " << itemId << " not found in your inventory.\n";
        }
    }

    void viewOrders(const std::vector<Invoice>& allInvoices, const std::vector<Buyer>& allBuyers) const {
        std::cout << "\n=== PAID ORDERS FOR " << storeName << " (ID: " << sellerId << ") ===\n";
        bool found = false;
        
        for (const auto& invoice : allInvoices) {
            if (invoice.getSellerId() == sellerId && invoice.isPaid()) {
                found = true;
                
                const Buyer* buyerDetails = nullptr;
                for (const auto& buyer : allBuyers) {
                    if (buyer.getId() == invoice.getBuyerId()) {
                        buyerDetails = &buyer;
                        break;
                    }
                }

                std::cout << "------------------------------------------------\n";
                std::cout << "Order/Invoice ID: " << invoice.getInvoiceId() << "\n";
                std::cout << "Status: PAID\n";
                
                if (buyerDetails) {
                    std::cout << "Buyer Name: " << buyerDetails->getName() << "\n";
                    std::cout << "Buyer Contact: " << buyerDetails->getEmail() << "\n";
                } else {
                    std::cout << "Buyer Details: [NOT FOUND, ID: " << invoice.getBuyerId() << "]\n";
                }
                
                std::cout << "Total Amount: $" << invoice.getTotalAmount() << "\n";
                invoice.printOrderItems();
            }
        }

        if (!found) {
            std::cout << "No paid orders found for this seller.\n";
            std::cout << "------------------------------------------------\n";
        }
    }

     void printInfo() const {
        std::cout << "Seller ID: " << sellerId << std::endl;
        std::cout << "Name: " << storeName << std::endl;
        std::cout << "Address: " << storeAddress << std::endl;
        std::cout << "Phone: " << storePhone << std::endl;
        std::cout << "Email: " << storeEmail << std::endl;
    }
};