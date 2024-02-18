#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>

using namespace std;

class BankAccount {
private:
    string ownerName;
    long accountNumber;
    double balance;

public:
    BankAccount() {}
    BankAccount(string name, long accNum, double initialBalance) {
        ownerName = name;
        accountNumber = accNum;
        balance = initialBalance;
    }

    void deposit(double amount) {
        balance += amount;
        cout << "You've added $" << fixed << setprecision(2) << amount << " to your account!" << endl;
    }

    void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            cout << "You've withdrawn $" << fixed << setprecision(2) << amount << "." << endl;
        } else {
            cout << "Sorry, you don't have enough funds." << endl;
        }
    }

    void checkBalance() {
        cout << "Your account balance is $" << fixed << setprecision(2) << balance << endl;
    }

    double getBalance() {
        return balance;
    }

    string getOwnerName() {
        return ownerName;
    }

    long getAccountNumber() {
        return accountNumber;
    }
};

void saveAccountInfo(const string& username, const string& password, double balance) {
    ofstream outFile("accounts.txt", ios::app);
    if (outFile.is_open()) {
        outFile << username << " " << password << " " << balance << endl;
        outFile.close();
    } else {
        cerr << "Oops! Couldn't write to the file." << endl;
        exit(1);
    }
}

bool authenticateUser(const string& username, const string& password, double& balance) {
    ifstream inFile("accounts.txt");
    string line;
    while (getline(inFile, line)) {
        size_t pos = line.find(' ');
        string storedUsername = line.substr(0, pos);
        line.erase(0, pos + 1);
        pos = line.find(' ');
        string storedPassword = line.substr(0, pos);
        if (username == storedUsername && password == storedPassword) {
            line.erase(0, pos + 1);
            balance = stod(line);
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

bool isUsernameAvailable(const string& username) {
    ifstream inFile("accounts.txt");
    string line;
    while (getline(inFile, line)) {
        size_t pos = line.find(' ');
        string storedUsername = line.substr(0, pos);
        if (username == storedUsername) {
            inFile.close();
            return false;
        }
    }
    inFile.close();
    return true;
}

int main() {
    int choice;
    string username, password;
    cout << "Welcome to Our Bank!" << endl;
    while (true) {
        cout << "\n1. Login\n2. Create Account\n3. Exit\nEnter your choice: ";
        cin >> choice;
        if (choice == 1) {
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;
            double balance;
            if (authenticateUser(username, password, balance)) {
                cout << "Welcome back!" << endl;
                BankAccount account(username, 1234567890, balance);
                while (true) {
                    cout << "\n1. Check Balance\n2. Deposit\n3. Withdraw\n4. Logout\nEnter your choice: ";
                    cin >> choice;
                    switch (choice) {
                        case 1:
                            account.checkBalance();
                            break;
                        case 2:
                            double amount;
                            cout << "Deposit amount: $";
                            cin >> amount;
                            account.deposit(amount);
                            break;
                        case 3:
                            cout << "Withdraw amount: $";
                            cin >> amount;
                            account.withdraw(amount);
                            break;
                        case 4:
                            cout << "Goodbye!" << endl;
                            saveAccountInfo(username, password, account.getBalance());
                            break;
                        default:
                            cout << "Invalid choice. Try again." << endl;
                    }
                    if (choice == 4) {
                        break;
                    }
                }
            } else {
                cout << "Login failed. Invalid username or password." << endl;
            }
        } else if (choice == 2) {
            cout << "Create a new account" << endl;
            cout << "Username: ";
            cin >> username;
            if (!isUsernameAvailable(username)) {
                cout << "Username already taken. Choose another one." << endl;
                continue;
            }
            cout << "Password: ";
            cin >> password;
            saveAccountInfo(username, password, 0.00);
            cout << "Account created. You can now log in." << endl;
        } else if (choice == 3) {
            cout << "Thank you for banking with us. Goodbye!" << endl;
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}
