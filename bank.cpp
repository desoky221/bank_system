#include <iostream>
#include <string>
#include <vector>

using namespace std;

class BankAccount {
protected:
    string Name;
    unsigned int AccNumber;
    unsigned int Id;
    char AccType;
    double Balance;
public:
    BankAccount(string name, unsigned int accnumber, unsigned int id, char acctype) :
            Name(name), AccNumber(accnumber), Id(id), AccType(acctype), Balance(100.0) {}

    ~BankAccount() {}

    void setId(unsigned int id) {
        Id = id;
    }

    void setAccType(char acctype) {
        AccType = acctype;
    }

    unsigned int getId() const {
        return Id;
    }

    void deposit(double amount) {
        if (amount < 1) {
            cout << "Make sure the deposited amount is more than 1$" << endl;
        } else {
            Balance += amount;
        }
    }

    void withdraw(double amount) {
        if (amount > Balance) {
            cout << "Insufficient balance" << endl;
        } else if (amount < 0) {
            cout << "Make sure to enter a non-negative amount" << endl;
        } else {
            Balance -= amount;
        }
    }

    void displayInfo() const {
        cout << "=========================" << endl;
        cout << "Name : " << Name << endl;
        cout << "Account number : " << AccNumber << endl;
        cout << "Account type : " << AccType << endl;
        cout << "Balance : " << Balance << endl;
        cout << "=========================" << endl;

    }
    friend ostream& operator<<(ostream& os, const BankAccount& account);
    friend void transferMoney(BankAccount& from, BankAccount& to, double amount);
};
ostream& operator<<(ostream& os, const BankAccount& account) {
    os << "=========================" << endl;
    os << "Name : " << account.Name << endl;
    os << "Account number : " << account.AccNumber << endl;
    os << "Account type : " << account.AccType << endl;
    os << "Balance : " << account.Balance << endl;
    os << "=========================" << endl;
    return os;
}

class SavingsAccount : public BankAccount {
private:
    double interestRate;
public:
    SavingsAccount(string name, unsigned int accnumber, unsigned int id, char acctype, double interest) :
            BankAccount(name, accnumber, id, acctype), interestRate(interest) {}
    ~SavingsAccount() {}

    void calculateInterest() {
        double interestAmount = Balance * (interestRate / 100);
        Balance += interestAmount;
        cout << "Interest calculated and added to account balance" << endl;
    }

    void displayInfo() const  {
        BankAccount::displayInfo();
        cout << "Interest Rate: " << interestRate << "%" << endl;
        cout << "=========================" << endl;
    }
};

class CurrentAccount : public BankAccount {
public:
    CurrentAccount(string name, unsigned int accnumber, unsigned int id)
            : BankAccount(name, accnumber, id, 'c') {}
    ~CurrentAccount() {}

    void withdraw(double amount)  {
        const double overdraftLimit = 500.0;
        if (amount > (Balance + overdraftLimit)) {
            cout << "Exceeds overdraft limit" << endl;
        } else {
            Balance -= amount;
        }
    }

    void displayInfo() const  {
        BankAccount::displayInfo();
        cout << "Account type: Current (overdraft limit $500)" << endl;
        cout << "=========================" << endl;
    }
};

void displayMenu();
int search(vector<BankAccount*>& Accounts);

void transferMoney(BankAccount& from, BankAccount& to, double amount) {
    if (amount <= 0) {
        cout << "Amount to transfer must be positive" << endl;
        return;
    }
    if (from.Balance < amount) {
        cout << "Insufficient balance in the source account" << endl;
        return;
    }
    from.Balance -= amount;
    to.Balance += amount;
    cout << "Transfer successful" << endl;
}

int main() {
    bool mainLoop = true;
    vector<BankAccount*> Accounts;
    unsigned int id, accnumber = 1000;
    string name;
    char acctype;
    double amount;
    int choice = 0;
    while (mainLoop) {
        displayMenu();
        cin >> choice;
        if (choice == 1) {
            cout << "Enter your name : " << endl;
            cin >> name;
            cout << "Enter id :" << endl;
            cin >> id;
            cout << "Enter account type ('c' for current account 's' for savings account):" << endl;
            cin >> acctype;
            if (acctype == 's' || acctype == 'c') {
                accnumber++;
                cout << "Your account number is: " << accnumber << endl;
                if (acctype == 's') {
                    double interestRate;
                    cout << "Enter interest rate for savings account: ";
                    cin >> interestRate;
                    SavingsAccount* newSavingsAcc = new SavingsAccount(name, accnumber, id, acctype, interestRate);
                    Accounts.push_back(newSavingsAcc);
                } else {
                    CurrentAccount* newCurrentAcc = new CurrentAccount(name, accnumber, id);
                    Accounts.push_back(newCurrentAcc);
                }
            } else {
                cout << "Invalid account type" << endl;
            }
        } else if (choice == 2) {
            int i = search(Accounts);
            if (i != -1) {
                cout << "Enter amount : " << endl;
                cin >> amount;
                Accounts[i]->deposit(amount);
            }
        } else if (choice == 3) {
            int i = search(Accounts);
            if (i != -1) {
                cout << "Enter amount : " << endl;
                cin >> amount;
                Accounts[i]->withdraw(amount);
            }
        } else if (choice == 4) {
            int i = search(Accounts);
            if (i != -1) {
                cout << "1. Modify account type" << endl;
                cout << "2. Modify account Id" << endl;
                cin >> choice;
                if (choice == 1) {
                    cout << "Enter new type " << endl;
                    cin >> acctype;
                    Accounts[i]->setAccType(acctype);
                } else if (choice == 2) {
                    cout << "Enter new Id " << endl;
                    cin >> id;
                    Accounts[i]->setId(id);
                }
            } else {
                cout << "invalid input" << endl;
            }
        } else if (choice == 5) {
            int i = search(Accounts);
            if (i != -1) {
                cout << *Accounts[i];
            }
        } else if (choice == 6) {
            int fromIndex = search(Accounts);
            int toIndex = search(Accounts);
            if (fromIndex != -1 && toIndex != -1) {
                cout << "Enter amount to transfer: ";
                cin >> amount;
                transferMoney(*Accounts[fromIndex], *Accounts[toIndex], amount);
            }
        } else if (choice == 7) {
            mainLoop = false;
        }
    }

    for (BankAccount* acc : Accounts) {
        delete acc;
    }

    return 0;
}

void displayMenu() {
    cout << endl << "--------------BankSystem--------------" << endl;
    cout << "1. Create Account" << endl;
    cout << "2. Deposit" << endl;
    cout << "3. Withdraw" << endl;
    cout << "4. Modify Account" << endl;
    cout << "5. Display info" << endl;
    cout << "6. Transfer Money" << endl;
    cout << "7. Exit" << endl;
    cout << "--------------------------------------" << endl;
}

int search(vector<BankAccount*>& Accounts) {
    int id;
    cout << "Enter Id : " << endl;
    cin >> id;
    for (int i = 0; i < Accounts.size(); i++) {
        if (Accounts[i]->getId() == id) {
            return i;
        }
    }
    cout << "Invalid id " << endl;
    return-1;
}
