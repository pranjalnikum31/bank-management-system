#include <iostream>
#include <unordered_map>
#include <string>
#include <ctime>
using namespace std;


struct Transaction {
    string type;
    double amount;
    time_t date;
    Transaction* next;

    Transaction(string t, double a) : type(t), amount(a), date(time(0)), next(nullptr) {}
};


class Account {
public:
    int accountNumber;
    string accountHolder;
    double balance;
    Transaction* transactionHead;

    // Constructor
    Account(int accNum, string name, double initialBalance = 0) : 
        accountNumber(accNum), accountHolder(name), balance(initialBalance), transactionHead(nullptr) {}

    // Deposit money
    void deposit(double amount) {
        balance += amount;
        addTransaction("Deposit", amount);
    }

    // Withdraw money
    bool withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance.\n";
            return false;
        }
        balance -= amount;
        addTransaction("Withdrawal", amount);
        return true;
    }

    // Add transaction to linked list
    void addTransaction(string type, double amount) {
        Transaction* newTransaction = new Transaction(type, amount);
        newTransaction->next = transactionHead;
        transactionHead = newTransaction;
    }

    // Display transaction history
    void displayTransactions() const {
        Transaction* current = transactionHead;
        while (current) {
            cout << current->type << ": " << current->amount 
                 << " on " << ctime(&current->date);
            current = current->next;
        }
    }
};

// Node for BST
struct AccountNode {
    Account* account;
    AccountNode* left;
    AccountNode* right;

    AccountNode(Account* acc) : account(acc), left(nullptr), right(nullptr) {}
};

// Bank Class
class Bank {
private:
    unordered_map<int, Account*> accountMap; 
    AccountNode* root;  

    
    AccountNode* insertToBST(AccountNode* node, Account* account) {
        if (!node) return new AccountNode(account);
        if (account->accountNumber < node->account->accountNumber)
            node->left = insertToBST(node->left, account);
        else
            node->right = insertToBST(node->right, account);
        return node;
    }

    // Helper for in-order traversal of BST
    void inOrderTraversal(AccountNode* node) const {
        if (!node) return;
        inOrderTraversal(node->left);
        cout << "Account Number: " << node->account->accountNumber
             << ", Account Holder: " << node->account->accountHolder
             << ", Balance: " << node->account->balance << endl;
        inOrderTraversal(node->right);
    }

public:
   
    Bank() : root(nullptr) {}

    
    void createAccount(int accountNumber, string name, double initialDeposit) {
        if (accountMap.find(accountNumber) != accountMap.end()) {
            cout << "Account with this number already exists.\n";
            return;
        }
        Account* newAccount = new Account(accountNumber, name, initialDeposit);
        accountMap[accountNumber] = newAccount;
        root = insertToBST(root, newAccount);
        cout << "Account created successfully.\n";
    }

   
    void deposit(int accountNumber, double amount) {
        if (accountMap.find(accountNumber) == accountMap.end()) {
            cout << "Account not found.\n";
            return;
        }
        accountMap[accountNumber]->deposit(amount);
        cout << "Deposited successfully.\n";
    }

    
    void withdraw(int accountNumber, double amount) {
        if (accountMap.find(accountNumber) == accountMap.end()) {
            cout << "Account not found.\n";
            return;
        }
        if (accountMap[accountNumber]->withdraw(amount))
            cout << "Withdrawal successful.\n";
    }

    
    void displayBalance(int accountNumber) const {
        if (accountMap.find(accountNumber) == accountMap.end()) {
            cout << "Account not found.\n";
            return;
        }
        cout << "Balance: " << accountMap.at(accountNumber)->balance << endl;
    }

   
    void displayTransactionHistory(int accountNumber) const {
        if (accountMap.find(accountNumber) == accountMap.end()) {
            cout << "Account not found.\n";
            return;
        }
        cout << "Transaction History for Account " << accountNumber << ":\n";
        accountMap.at(accountNumber)->displayTransactions();
    }

    
    void displayAllAccounts() const {
        cout << "All Accounts (sorted by Account Number):\n";
        inOrderTraversal(root);
    }
};

int main() {
    Bank bank;
    int choice, accountNumber;
    string name;
    double amount;

    while (true) {
        cout << "\n--- Bank Management System ---\n";
        cout << "1. Create Account\n2. Deposit\n3. Withdraw\n4. Check Balance\n5. Transaction History\n6. Display All Accounts\n7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Account Holder Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Initial Deposit: ";
                cin >> amount;
                bank.createAccount(accountNumber, name, amount);
                break;
            case 2:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Amount to Deposit: ";
                cin >> amount;
                bank.deposit(accountNumber, amount);
                break;
            case 3:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Amount to Withdraw: ";
                cin >> amount;
                bank.withdraw(accountNumber, amount);
                break;
            case 4:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                bank.displayBalance(accountNumber);
                break;
            case 5:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                bank.displayTransactionHistory(accountNumber);
                break;
            case 6:
                bank.displayAllAccounts();
                break;
            case 7:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
