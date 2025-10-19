#include <iostream>
#include <vector>
#include "bank_customer.h"
#include "buyer.h"
#include "seller.h"
#include "bank.h"
#include "invoice.h"
#include "cartEntry.h"


using namespace std;

enum PrimaryPrompt { LOGIN, REGISTER, EXIT, ADMIN_LOGIN };
enum RegisterPrompt { CREATE_BUYER, CREATE_SELLER, BACK };

void displayBuyerDetails(const Buyer& b) {
    cout << "ID       : " << b.getId() << endl;
    cout << "Name     : " << b.getName() << endl;
    cout << "Address  : " << b.getAddress() << endl;
    cout << "Phone    : " << b.getPhone() << endl;
    cout << "Email    : " << b.getEmail() << endl;
    cout << "Bank Acc : " << b.getAccount().getId() << " (Balance: $" << b.getAccount().getBalance() << ")" << endl;
}

void displaySellerDetails(const seller& s) {
    cout << "Seller ID: " << s.getSellerId() << endl;
    cout << "Store    : " << s.getStoreName() << endl;
    cout << "Owner    : " << s.getName() << endl;
    cout << "Address  : " << s.getStoreAddress() << endl;
    cout << "Phone    : " << s.getStorePhone() << endl;
    cout << "Email    : " << s.getStoreEmail() << endl;
    cout << "Deposit  : $" << s.getDeposit() << endl;
    cout << "Bank Acc : " << s.getAccount().getId() << " (Balance: $" << s.getAccount().getBalance() << ")" << endl;
}

// for searching users
string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

void removeBuyer(vector<Buyer>& buyers, vector<seller>& sellers, Bank& bank, int buyerId) {
    auto it = remove_if(buyers.begin(), buyers.end(), 
        [&](const Buyer& b) {
            if (b.getId() == buyerId) {
                if (b.hasBankAccount()) {
                    bank.removeAccount(b.getAccount().getId());
                }
                return true; 
            }
            return false;
        });

    if (it != buyers.end()) {
        buyers.erase(it, buyers.end());
        cout << "Buyer ID " << buyerId << " and related bank account successfully removed.\n";
    } else {
        cout << "Buyer ID " << buyerId << " not found.\n";
    }
}

void removeSeller(vector<seller>& sellers, Bank& bank, int sellerId) {
    auto it = remove_if(sellers.begin(), sellers.end(), 
        [&](const seller& s) {
            if (s.getId() == sellerId) {
                if (s.hasBankAccount()) {
                    bank.removeAccount(s.getAccount().getId());
                }
                return true;
            }
            return false;
        });

    if (it != sellers.end()) {
        sellers.erase(it, sellers.end());
        cout << "Seller ID " << sellerId << " and related bank account successfully removed.\n";
    } else {
        cout << "Seller ID " << sellerId << " not found.\n";
    }
}

int main() {
    PrimaryPrompt prompt = LOGIN;
    RegisterPrompt regPrompt = CREATE_BUYER;
    const string ADMIN_USERNAME = "root";
    const string ADMIN_PASSWORD = "toor";
    string username, password;

    vector<Buyer> buyers;
    vector<seller> sellers;
    vector<Invoice> invoices;
    int nextInvoiceId = 1;
    int nextBuyerId = 1;
    int nextSellerId = 1;

    Bank bank("Main Bank");

    // ====================== MAIN MENU ========================
    while (prompt != EXIT) {
        cout << "\n=== MAIN MENU ===" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
        cout << "Select: ";
        int choice;
        cin >> choice;

        prompt = static_cast<PrimaryPrompt>(choice - 1);
        cout << endl;

        switch (prompt) {
            // =================== LOGIN ===================
            case LOGIN: {
                cout << "=== Login ===" << endl;
                cout << "1. Buyer" << endl;
                cout << "2. Seller" << endl;
                cout << "3. Back" << endl;
                int loginChoice;
                cin >> loginChoice;
                if (loginChoice == 3) break;

                int id;
                cout << "Enter ID: ";
                cin >> id;

                bool loggedIn = false;

                // ================== LOGIN BUYER ==================
                if (loginChoice == 1) {
                    for (auto &b : buyers) {
                        if (b.getId() == id) {
                            loggedIn = true;
                            vector<CartEntry> cart;
                            cout << "Logged in as Buyer: " << b.getName() << endl;
                            
                            bool buyerExit = false;
                            while (!buyerExit) {
                                cout << "\n=== BUYER MENU ===" << endl;
                                cout << "1. View Profile" << endl;
                                cout << "2. Upgrade to Seller" << endl;
                                cout << "3. Bank Menu" << endl;
                                cout << "4. Browse store" << endl;
                                cout << "5. View cart" << endl;
                                cout << "6. payment" << endl;
                                cout << "7. Logout" << endl;
                                cout << "8. Delete Account" << endl;
                                cout << "Select: ";
                                int bchoice;
                                cin >> bchoice;

                                switch (bchoice) {
                                    // ============= PROFILE =================
                                    case 1: {
                                        cout << "\n=== Buyer Information ===\n";
                                        cout << "ID        : " << b.getId() << endl;
                                        cout << "Name      : " << b.getName() << endl;
                                        cout << "Address   : " << b.getAddress() << endl;
                                        cout << "Phone     : " << b.getPhone() << endl;
                                        cout << "Email     : " << b.getEmail() << endl;
                                        if (b.hasBankAccount())
                                            cout << "Balance   : $" << b.getAccount().getBalance() << endl;
                                        else
                                            cout << "No bank account linked.\n";
                                        break;
                                    }

                                    case 2: {
                                        if (!b.hasBankAccount()) {
                                            cout << "❌ You must create a bank account first!\n";
                                            break;
                                        }

                                        cout << "\n=== Upgrade to Seller ===\n";
                                        string storeName, storeAddress, storePhone, storeEmail;
                                        double deposit;

                                        cin.ignore();
                                        cout << "Store name: ";
                                        getline(cin, storeName);
                                        cout << "Store address: ";
                                        getline(cin, storeAddress);
                                        cout << "Store phone: ";
                                        getline(cin, storePhone);
                                        cout << "Store email: ";
                                        getline(cin, storeEmail);
                                        cout << "Initial deposit: ";
                                        cin >> deposit;

                                        if (b.getAccount().withdrawBalance(deposit)) {
                                            seller newSeller(b, nextSellerId, storeName, storeAddress, storePhone, storeEmail, deposit);
                                            sellers.push_back(newSeller);
                                            nextSellerId++;
                                            cout << "Seller account created successfully!\n";
                                        } else {
                                            cout << "Not enough balance to deposit.\n";
                                        }
                                        break;
                                    }

                                    // ================ BANK MENU =======================
                                    case 3: { 
                                        bool bankExit = false;
                                        while (!bankExit) {
                                            cout << "\n=== BANK MENU ===" << endl;
                                            cout << "1. Create Bank Account" << endl;
                                            cout << "2. Check Balance" << endl;
                                            cout << "3. Deposit" << endl;
                                            cout << "4. Withdraw" << endl;
                                            cout << "5. Back" << endl;
                                            cout << "Select: ";
                                            int bankChoice;
                                            cin >> bankChoice;

                                            switch (bankChoice) {
                                                case 1: {
                                                    if (b.hasBankAccount()) {
                                                        cout << "You already have a bank account!\n";
                                                        break;
                                                    }

                                                    string cname;
                                                    double deposit;
                                                    cin.ignore();
                                                    cout << "Enter account name: ";
                                                    getline(cin, cname);
                                                    cout << "Initial deposit: ";
                                                    cin >> deposit;

                                                    BankCustomer newAccount(b.getId(), cname, deposit);
                                                    b.setAccount(newAccount);
                                                    cout << "Bank account created successfully!\n";
                                                    break;
                                                }
                                                case 2: {
                                                    if (!b.hasBankAccount()) {
                                                        cout << "Please create a bank account first.\n";
                                                        break;
                                                    }
                                                    cout << "Current Balance: $" << b.getAccount().getBalance() << endl;
                                                    break;
                                                }
                                                case 3: {
                                                    if (!b.hasBankAccount()) {
                                                        cout << "Please create a bank account first.\n";
                                                        break;
                                                    }
                                                    double amount;
                                                    cout << "Deposit amount: ";
                                                    cin >> amount;
                                                    b.getAccount().addBalance(amount);
                                                    cout << "Deposit successful!\n";
                                                    break;
                                                }
                                                case 4: {
                                                    if (!b.hasBankAccount()) {
                                                        cout << "Please create a bank account first.\n";
                                                        break;
                                                    }
                                                    double amount;
                                                    cout << "Withdraw amount: ";
                                                    cin >> amount;
                                                    if (b.getAccount().withdrawBalance(amount))
                                                        cout << "Withdrawal successful!\n";
                                                    else
                                                        cout << "Insufficient balance.\n";
                                                    break;
                                                }
                                                case 5:
                                                    bankExit = true;
                                                    break;
                                                default:
                                                    cout << "Invalid choice.\n";
                                            }
                                        }
                                        break;
                                    }

                                    // ======= Browse store =======
                                    case 4: {
                                        if (sellers.empty()) {
                                            cout << "No stores available.\n";
                                            break;
                                        }

                                        cout << "\n=== STORE LIST ===\n";
                                        for (size_t i = 0; i < sellers.size(); ++i) {
                                            cout << (i+1) << ". " << sellers[i].getStoreName()
                                                << " (Owner: " << sellers[i].getName() << ")\n";
                                        }

                                        cout << "Select store number to browse (0 to cancel): ";
                                        int storeChoice;
                                        cin >> storeChoice;
                                        if (storeChoice == 0) break;
                                        if (storeChoice < 1 || storeChoice > (int)sellers.size()) {
                                            cout << "Invalid store selection.\n";
                                            break;
                                        }

                                        seller &chosen = sellers[storeChoice - 1];
                                        cout << "\n=== ITEMS IN " << chosen.getStoreName() << " ===\n";
                                        chosen.displayItems();

                                        cout << "\nEnter Item ID to view details (0 to cancel): ";
                                        int itemId;
                                        cin >> itemId;
                                        if (itemId == 0) break;

                                        const auto& items = chosen.getItems();
                                        const Item* found = nullptr;
                                        for (const auto& it : items) {
                                            if (it.getId() == itemId) { found = &it; break; }
                                        }

                                        if (!found) {
                                            cout << "Item not found in this store.\n";
                                            break;
                                        }

                                        cout << "\n=== ITEM DETAILS ===\n";
                                        cout << "ID: " << found->getId() << "\n";
                                        cout << "Name: " << found->getName() << "\n";
                                        cout << "Quantity available: " << found->getQuantity() << "\n";
                                        cout << "Price per unit: $" << found->getPrice() << "\n";

                                        cout << "\nAdd to cart? (y/n): ";
                                        char addc; 
                                        cin >> addc;
                                        if (addc == 'y' || addc == 'Y') {
                                            int qty;
                                            cout << "Enter quantity to add: ";
                                            cin >> qty;
                                            if (qty <= 0) {
                                                cout << "Invalid quantity.\n";
                                                break;
                                            }
                                            if (qty > found->getQuantity()) {
                                                cout << "Not enough stock. Available: " << found->getQuantity() << "\n";
                                                break;
                                            }
                                        
                                            CartEntry entry(
                                                found->getId(),
                                                found->getName(),
                                                found->getPrice(),
                                                qty,
                                                chosen.getSellerId()
                                            );
                                            cart.push_back(entry);

                                            cout << "Item added to cart: " << entry.getItemName() << " x" << qty << "\n";

                                        }
                                        break;
                                    }

                                    // ========== CART ===========
                                    case 5: {
                                        if (cart.empty()) {
                                            cout << "Your cart is empty.\n";
                                            break;
                                        }
                                        cout << "\n=== YOUR CART ===\n";
                                        double total = 0.0;
                                        for (size_t i = 0; i < cart.size(); ++i) {
                                            double line = cart[i].getPrice() * cart[i].getQuantity();
                                            cout << (i+1) << ". " << cart[i].getItemName() << " x" << cart[i].getQuantity()
                                                << " = $" << line << "\n";
                                            total += line;
                                        }
                                        cout << "Total: $" << total << "\n";
                                        cout << "1. Remove item\n2. Checkout (generate invoice)\n3. Back\nSelect: ";
                                        int opt; cin >> opt;
                                        if (opt == 1) {
                                            cout << "Enter item number to remove: ";
                                            int rem; cin >> rem;
                                            if (rem >= 1 && rem <= (int)cart.size()) {
                                                cout << "Removed " << cart[rem-1].getItemName() << "\n";
                                                cart.erase(cart.begin() + (rem-1));
                                            } else cout << "Invalid index.\n";
                                        } else if (opt == 2) {
                                            if (cart.empty()) {
                                                cout << "Cart is empty!\n";
                                                break;
                                            }

                                            cout << "Select seller ID for this purchase: ";
                                            int sid; cin >> sid;
                                            bool foundSeller = false;
                                            for (auto& s : sellers) {
                                                if (s.getSellerId() == sid) {
                                                    foundSeller = true;
                                                    Invoice newInvoice(nextInvoiceId++, b.getId(), s.getSellerId(), cart);
                                                    invoices.push_back(newInvoice);
                                                    cout << "Invoice #" << newInvoice.getInvoiceId() << " created for seller " << s.getStoreName() << "!\n";
                                                    cart.clear();
                                                    break;
                                                }
                                            }
                                            if (!foundSeller) cout << "Seller not found.\n";
                                        }

                                        break;
                                    }
                                        
                                    
                                    case 6: {
                                        cout << "\n=== PAYMENT MENU ===\n";
                                        bool hasInvoice = false;
                                        for (const auto& inv : invoices) {
                                            if (inv.getBuyerId() == b.getId()) {
                                                inv.printInvoice();
                                                hasInvoice = true;
                                            }
                                        }

                                        if (!hasInvoice) {
                                            cout << "No invoices found.\n";
                                            break;
                                        }

                                        cout << "Enter invoice ID to pay (0 to cancel): ";
                                        int payId; cin >> payId;
                                        if (payId == 0) break;

                                        for (auto& inv : invoices) {
                                            if (inv.getInvoiceId() == payId && inv.getBuyerId() == b.getId()) {
                                                if (inv.isPaid()) {
                                                    cout << "Invoice already paid!\n";
                                                    break;
                                                }

                                                if (!b.hasBankAccount()) {
                                                    cout << "You need a bank account to make payments!\n";
                                                    break;
                                                }

                                                double buyerBalance = b.getAccount().getBalance();
                                                double amount = inv.getTotalAmount();
                                                cout << "Invoice total: $" << amount << "\n";
                                                cout << "Your balance: $" << buyerBalance << "\n";

                                                if (buyerBalance < amount) {
                                                    cout << "Insufficient funds!\n";
                                                    break;
                                                }

                                                cout << "Enter invoice ID again to confirm payment: ";
                                                int confirm; cin >> confirm;
                                                if (confirm != payId) {
                                                    cout << "Payment cancelled.\n";
                                                    break;
                                                }

                                                for (auto& s : sellers) {
                                                    if (s.getSellerId() == inv.getSellerId()) {
                                                        s.getAccount().deposit(amount);
                                                        break;
                                                    }
                                                }

                                                b.getAccount().withdraw(amount);
                                                inv.printInvoice();
                                                cout << "Payment successful! $" << amount << " deducted from your account.\n";
                                                break;
                                            }
                                        }
                                        break;
                                    }

                                    case 7:
                                        buyerExit = true;
                                        cout << "Logging out...\n";
                                        break;

                                    case 8: { //  Delete Account
                                        char confirm;
                                        cout << "\nDANGER: Are you sure you want to delete your account (ID: " << b.getId() << ")?\n";
                                        cout << "This action is permanent and will delete related bank account.\n";
                                        cout << "Type 'Y' to confirm deletion, any other key to cancel: ";
                                        cin >> confirm;

                                        if (confirm == 'Y' || confirm == 'y') {
                                            int buyerIdToDelete = b.getId();
                                            
                                            removeBuyer(buyers, sellers, bank, buyerIdToDelete);
                                            
                                            buyerExit = true; 
                                            loggedIn = false; 
                                            cout << "Account deleted successfully. Returning to Main Menu.\n";
                                        } else {
                                            cout << "Deletion cancelled. Returning to Buyer Menu.\n";
                                        }
                                        break;
                                    }

                                    default:
                                        cout << "Invalid choice.\n";
                                }
                            }
                        }
                    }
                }

                // LOGIN SELLER
                if (loginChoice == 2) {
                    for (auto &s : sellers) {
                        if (s.getId() == id) {
                            loggedIn = true;
                            cout << "Logged in as Seller: " << s.getName() << endl;
                            bool sellerExit = false;
                            while (!sellerExit) {
                                cout << "\n=== SELLER MENU ===" << endl;
                                cout << "1. Seller Information" << endl;
                                cout << "2. View Inventory" << endl;
                                cout << "3. Add Item" << endl;
                                cout << "4. Remove Item" << endl;
                                cout << "5. View Orders" << endl;
                                cout << "6. Logout" << endl;
                                cout << "7. Delete Account" << endl;
                                cout << "select: ";
                                int schoice;
                                cin >> schoice;

                                switch (schoice) {
                                    case 1:
                                        cout << "\n=== SELLER INFORMATION ===\n";
                                        s.printInfo();
                                        cout << "Balance   : $" << s.getAccount().getBalance() << endl;
                                        break;
                                    case 2:
                                        s.displayItems();
                                        break;
                                    case 3: {
                                        int iid, qty;
                                        string iname;
                                        double price;
                                        cout << "Item ID: ";
                                        cin >> iid;
                                        cout << "Item Name: ";
                                        cin >> ws;
                                        getline(cin, iname);
                                        cout << "Quantity: ";
                                        cin >> qty;
                                        cout << "Price: ";
                                        cin >> price;
                                        s.addNewItem(iid, iname, qty, price);
                                        cout << "Item added successfully.\n";
                                        break;
                                    }

                                    case 4: { // Remove Item
                                        int itemId;
                                        cout << "\n=== REMOVE ITEM ===\n";
                                        s.displayItems();
                                        cout << "Enter Item ID to remove: ";
                                        cin >> itemId;
                                        s.removeItemFromInventory(itemId);
                                        break;
                                    }
                                    case 5: { // View Orders
                                        s.viewOrders(invoices, buyers);
                                        break;
                                    }
                                    case 6:
                                        sellerExit = true;
                                        cout << "Logging out seller...\n";
                                        break;

                                    case 7: { //  Delete Account
                                        char confirm;
                                        cout << "\nDANGER: Are you sure you want to delete your Seller account (ID: " << s.getId() << ")?\n";
                                        cout << "This action is permanent and will delete inventory and related bank account.\n";
                                        cout << "Type 'Y' to confirm deletion, any other key to cancel: ";
                                        cin >> confirm;

                                        if (confirm == 'Y' || confirm == 'y') {
                                            int sellerIdToDelete = s.getId();
                                            
                                            removeSeller(sellers, bank, sellerIdToDelete);
                                            
                                            sellerExit = true; 
                                            loggedIn = false; 
                                            cout << "Account deleted successfully. Returning to Main Menu.\n";
                                        } else {
                                            cout << "Deletion cancelled. Returning to Seller Menu.\n";
                                        }
                                        break;
                                    }

                                    default:
                                        cout << "Invalid choice.\n";
                                }
                            }
                        }
                    }
                }

                if (!loggedIn) cout << "Account not found.\n";
                break;
            }

            // =================== REGISTER ===================
            case REGISTER: {
                regPrompt = CREATE_BUYER;
                while (regPrompt != BACK) {
                    cout << "\n=== REGISTER MENU ===" << endl;
                    cout << "1. Create Buyer Account" << endl;
                    cout << "2. Create Seller Account" << endl;
                    cout << "3. Back" << endl;
                    cout << "Select: ";
                    int regChoice;
                    cin >> regChoice;
                    regPrompt = static_cast<RegisterPrompt>(regChoice - 1);

                    switch (regPrompt) {
                        case CREATE_BUYER: {
                            cout << "\n=== Create Buyer Account ===" << endl;
                            string name, address, phone, email;
                            cout << "Enter name: ";
                            cin >> ws;
                            getline(cin, name);
                            cout << "Enter address: ";
                            getline(cin, address);
                            cout << "Enter phone: ";
                            getline(cin, phone);
                            cout << "Enter email: ";
                            getline(cin, email);

                            BankCustomer newCustomer(nextBuyerId, name, 0.0);
                            Buyer newBuyer(nextBuyerId, name, address, phone, email, newCustomer);
                            buyers.push_back(newBuyer);
                            cout << "Buyer account created (ID: " << nextBuyerId << ")\n";
                            nextBuyerId++;
                            break;
                        }

                        case CREATE_SELLER: {
                            cout << "\n=== Create Seller Account ===" << endl;

                            string name, address, phone, email;
                            cout << "Enter name: ";
                            cin >> ws;
                            getline(cin, name);
                            cout << "Enter address: ";
                            getline(cin, address);
                            cout << "Enter phone: ";
                            getline(cin, phone);
                            cout << "Enter email: ";
                            getline(cin, email);

                            bool buyerExists = false;
                            Buyer* existingBuyer = nullptr;

                            for (auto& b : buyers) {
                                if (b.getEmail() == email) {
                                    buyerExists = true;
                                    existingBuyer = &b;
                                    break;
                                }
                            }

                            if (buyerExists) {
                                cout << "Buyer account already exists.\n";

                                bool alreadySeller = false;
                                for (const auto& s : sellers) {
                                    if (s.getEmail() == email) {
                                        alreadySeller = true;
                                        break;
                                    }
                                }

                                if (alreadySeller) {
                                    cout << "❌ You already registered as a Seller!\n";
                                    break;
                                }

                                if (!existingBuyer->hasBankAccount()) {
                                    string cname;
                                    double deposit;
                                    cin.ignore();
                                    cout << "Enter account name: ";
                                    getline(cin, cname);
                                    cout << "Initial deposit: ";
                                    cin >> deposit;

                                    BankCustomer newAccount(existingBuyer->getId(), cname, deposit);
                                    existingBuyer->setAccount(newAccount);
                                    cout << "Bank account created successfully!\n";
                                } else {
                                    cout << "You already have a bank account.\n";
                                }

                                cout << "\n=== Seller Store Information ===" << endl;
                                string storeName, storeAddress, storePhone, storeEmail;
                                double deposit;

                                cin.ignore();
                                cout << "Store Name: ";
                                getline(cin, storeName);
                                cout << "Store Address: ";
                                getline(cin, storeAddress);
                                cout << "Store Phone: ";
                                getline(cin, storePhone);
                                cout << "Store Email: ";
                                getline(cin, storeEmail);
                                cout << "Initial Deposit: ";
                                cin >> deposit;

                                seller newSeller(
                                    *existingBuyer,
                                    nextSellerId,
                                    storeName,
                                    storeAddress,
                                    storePhone,
                                    storeEmail,
                                    deposit
                                );

                                sellers.push_back(newSeller);
                                cout << "Seller account created successfully!" << endl;
                                cout << "Seller ID: " << nextSellerId << endl;
                                nextSellerId++;
                            }

                            else {                            
                                cout << "\nCreating new Buyer account..." << endl;
                                BankCustomer newCustomer(nextBuyerId, name, 0.0);
                                Buyer newBuyer(nextBuyerId, name, address, phone, email, newCustomer);

                                cout << "Creating Bank Account" << endl;
                                string cname;
                                double deposit;
                                cout << "Enter account name: ";
                                getline(cin, cname);
                                cout << "Initial deposit: ";
                                cin >> deposit;

                                BankCustomer newAccount(newBuyer.getId(), cname, deposit);
                                newBuyer.setAccount(newAccount);
                                cout << "Bank account created successfully!\n";

                                cout << "\n=== Seller Store Information ===" << endl;
                                string storeName, storeAddress, storePhone, storeEmail;
                                double sDeposit;

                                cin.ignore();
                                cout << "Store Name: ";
                                getline(cin, storeName);
                                cout << "Store Address: ";
                                getline(cin, storeAddress);
                                cout << "Store Phone: ";
                                getline(cin, storePhone);
                                cout << "Store Email: ";
                                getline(cin, storeEmail);
                                cout << "Initial Deposit: ";
                                cin >> sDeposit;

                                seller newSeller(
                                    newBuyer,
                                    nextSellerId,
                                    storeName,
                                    storeAddress,
                                    storePhone,
                                    storeEmail,
                                    sDeposit
                                );

                                buyers.push_back(newBuyer);
                                sellers.push_back(newSeller);

                                cout << "Seller account created successfully!" << endl;
                                cout << "Seller ID: " << nextSellerId << endl;

                                nextBuyerId++;
                                nextSellerId++;
                            }

                            break;
                        }


                        case BACK:
                            break;
                        default:
                            cout << "Invalid option.\n";
                    }
                }
                break;
            }

            // =================== ADMIN LOGIN ===================
            case ADMIN_LOGIN: {
                cout << "=== Admin Login ===" << endl;
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;

                if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
                    cout << "Admin login successful.\n";
                    bool adminExit = false;
                    while (!adminExit) {
                        cout << "\n=== ADMIN MENU ===" << endl;
                        cout << "1. View All Buyer & Seller Details" << endl;
                        cout << "2. Seek Buyer and Seller" << endl;
                        cout << "3. Create New Buyer / Seller Accounts" << endl;
                        cout << "4. Remove Buyer / Seller Accounts" << endl;
                        cout << "5. View All Bank Accounts" << endl;
                        cout << "6. Logout" << endl;
                        cout << "Select: ";
                        int achoice;
                        cin >> achoice;

                        switch (achoice) {
                            case 1: { 
                                cout << "\n=== ALL BUYER DETAILS ===\n";
                                if (buyers.empty()) cout << "No buyers.\n";
                                for (const auto &b : buyers) {
                                    displayBuyerDetails(b);
                                    cout << "-------------------------\n";
                                }

                                cout << "\n=== ALL SELLER DETAILS ===\n";
                                if (sellers.empty()) cout << "No sellers.\n";
                                for (const auto &s : sellers) {
                                    displaySellerDetails(s);
                                    cout << "-------------------------\n";
                                }
                                break;
                            }
                            case 2: { // Seek Buyer/Seller
                                cout << "\n=== SEEK BUYER/SELLER ===\n";
                                cout << "Enter search term (Name/ID/Address/Phone/Email): ";
                                string searchTerm;
                                cin >> ws;
                                getline(cin, searchTerm);
                                string lowerSearch = toLower(searchTerm);
                                bool found = false;

                                cout << "\n-- Buyers Found --\n";
                                for (const auto &b : buyers) {
                                    string accIdStr = to_string(b.getAccount().getId());
                                    string buyerIdStr = to_string(b.getId());
                                    if (toLower(b.getName()).find(lowerSearch) != string::npos ||
                                        toLower(b.getAddress()).find(lowerSearch) != string::npos ||
                                        b.getPhone() == searchTerm || b.getEmail() == searchTerm ||
                                        accIdStr == searchTerm || buyerIdStr == searchTerm) {
                                        displayBuyerDetails(b);
                                        cout << "-------------------------\n";
                                        found = true;
                                    }
                                }
                             
                                cout << "\n-- Sellers Found --\n";
                                for (const auto &s : sellers) {
                                    string accIdStr = to_string(s.getAccount().getId());
                                    string sellerIdStr = to_string(s.getId());
                                    if (toLower(s.getName()).find(lowerSearch) != string::npos ||
                                        toLower(s.getStoreName()).find(lowerSearch) != string::npos ||
                                        toLower(s.getStoreAddress()).find(lowerSearch) != string::npos ||
                                        s.getStorePhone() == searchTerm || s.getStoreEmail() == searchTerm ||
                                        accIdStr == searchTerm || sellerIdStr == searchTerm) {
                                        displaySellerDetails(s);
                                        cout << "-------------------------\n";
                                        found = true;
                                    }
                                }

                                if (!found) cout << "No matching Buyer or Seller found.\n";
                                break;
                            }
                            case 3: { 
                                cout << "\n=== ADMIN - CREATE NEW ACCOUNT ===\n";
                                cout << "1. Create Buyer\n";
                                cout << "2. Create Seller\n";
                                cout << "Select: ";
                                int createChoice;
                                cin >> createChoice;

                                if (createChoice == 1 || createChoice == 2) {
                                  
                                    string name, address, phone, email;
                                    double initialDeposit;
                                    
                                    cout << "Name: "; cin >> ws; getline(cin, name);
                                    cout << "Address: "; getline(cin, address);
                                    cout << "Phone: "; getline(cin, phone);
                                    cout << "Email: "; getline(cin, email);
                                    cout << "Initial Deposit: $"; cin >> initialDeposit;

                                    BankCustomer newBankAcc = bank.createAccount(name, initialDeposit);
                                    
                                    if (createChoice == 1) { 
                                        Buyer newBuyer(nextBuyerId++, name, address, phone, email, newBankAcc);
                                        newBuyer.setAccount(newBankAcc);
                                        buyers.push_back(newBuyer);
                                        cout << "New Buyer created successfully! ID: " << newBuyer.getId() << endl;
                                    } else { 
                                        string storeName;
                                        cout << "Store Name: "; cin >> ws; getline(cin, storeName);
                                        seller newSeller(Buyer(nextSellerId, name, address, phone, email, newBankAcc), 
                                                        nextSellerId++, storeName, address, phone, email, initialDeposit);
                                        newSeller.setAccount(newBankAcc);
                                        sellers.push_back(newSeller);
                                        cout << "New Seller created successfully! ID: " << newSeller.getId() << endl;
                                    }
                                } else {
                                    cout << "Invalid choice.\n";
                                }
                                break;
                            }
                            case 4: { 
                                cout << "\n=== REMOVE BUYER/SELLER ===\n";
                                cout << "1. Remove Buyer\n";
                                cout << "2. Remove Seller\n";
                                cout << "Select: ";
                                int removeChoice;
                                cin >> removeChoice;
                                
                                if (removeChoice == 1) {
                                    int id;
                                    cout << "Enter Buyer ID to remove: ";
                                    cin >> id;
                                    removeBuyer(buyers, sellers, bank, id);
                                } else if (removeChoice == 2) {
                                    int id;
                                    cout << "Enter Seller ID to remove: ";
                                    cin >> id;
                                    removeSeller(sellers, bank, id);
                                } else {
                                    cout << "Invalid choice.\n";
                                }
                                break;
                            }
                            case 5: 
                                bank.displayAllAccounts();
                                break;

                            case 6:
                                adminExit = true;
                                cout << "Logging out admin...\n";
                                break;
                            default:
                                cout << "Invalid admin option.\n";
                        }
                    }
                } else {
                    cout << "Invalid admin credentials.\n";
                }
                break;
            }

            case EXIT:
                cout << "Exiting program...\n";
                break;
        }
    }

    return 0;
}