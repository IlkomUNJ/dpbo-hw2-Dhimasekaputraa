#ifndef INVOICE_H
#define INVOICE_H

#include <vector>
#include <string>
#include <iostream>
#include "cartEntry.h"

using namespace std;

class Invoice {
private:
    int invoiceId;
    int buyerId;
    int sellerId;
    double totalAmount;
    bool paid;
    vector<CartEntry> items;

public:
    Invoice() : invoiceId(0), buyerId(0), sellerId(0), totalAmount(0), paid(false) {}

    Invoice(int invId, int bId, int sId, const vector<CartEntry>& cartEntries)
        : invoiceId(invId), buyerId(bId), sellerId(sId), totalAmount(0), paid(false) 
    {
        // salin semua CartEntry ke dalam invoice
        for (const auto& entry : cartEntries) {
            items.push_back(entry);
            totalAmount += entry.getTotalPrice();
        }
    }

    int getInvoiceId() const { return invoiceId; }
    int getBuyerId() const { return buyerId; }
    int getSellerId() const { return sellerId; }
    double getTotalAmount() const { return totalAmount; }
    bool isPaid() const { return paid; }

    void markAsPaid() { paid = true; }

    void printOrderItems() const {
        cout << "Items:\n";
        for (const auto& entry : items) {
            cout << "  - " << entry.getItemName() 
                 << " (Qty: " << entry.getQuantity() 
                 << ", Price/unit: $" << entry.getPrice() 
                 << ", Subtotal: $" << entry.getTotalPrice() << ")\n";
        }
    }

    void printInvoice() const {
        cout << "==================== INVOICE ====================\n";
        cout << "Invoice ID : " << invoiceId << "\n";
        cout << "Buyer ID   : " << buyerId << "\n";
        cout << "Seller ID  : " << sellerId << "\n";
        cout << "-----------------------------------------------\n";

        for (const auto& entry : items) {
            cout << entry.getItemName() << " x" << entry.getQuantity()
                 << " - $" << entry.getPrice() << " each"
                 << " | Subtotal: $" << entry.getTotalPrice() << "\n";
        }

        cout << "-----------------------------------------------\n";
        cout << "Total Amount: $" << totalAmount << "\n";
        cout << "Status: " << (paid ? "PAID" : "UNPAID") << "\n";
        cout << "================================================\n";
    }
};

#endif
