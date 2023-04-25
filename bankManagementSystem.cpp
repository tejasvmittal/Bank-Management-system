#include "bankManagementSystem.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
using namespace ICS45C::BankManagementSystem;

Record *helper_findaccount(Record *accounts, unsigned int id) {
  while (accounts != nullptr) {
    if (accounts->id == id) {
      return accounts;
    }
    accounts = accounts->next;
  }
  return nullptr;
}

Record *ICS45C::BankManagementSystem::openAccount(Record *accounts,
                                                  std::string fname,
                                                  std::string lname,
                                                  std::string email,
                                                  std::string phone) {
  static unsigned int ACCOUNT_ID = 0;
  Record *new_account = new Record();
  Record *head = accounts;
  new_account->fname = fname;
  new_account->lname = lname;
  new_account->email = email;
  new_account->phone = phone;
  new_account->locked = false;
  new_account->balance = 0.0;
  new_account->id = ACCOUNT_ID;
  new_account->next = nullptr;  // what if the head itself is a nullptr
  if (head == nullptr) {
    head = new_account;
  } else {
    while (accounts->next != nullptr) {
      accounts = accounts->next;
    }

    accounts->next = new_account;
  }

  ACCOUNT_ID += 1;
  return head;
}

void ICS45C::BankManagementSystem::lockAccount(Record *account) {
  if (account != nullptr) {
    account->locked = true;
  }
}

void ICS45C::BankManagementSystem::unlockAccount(Record *account) {
  if (account != nullptr) {
    account->locked = false;
  }
}

Record *ICS45C::BankManagementSystem::searchAccount(Record *accounts,
                                                    std::string field,
                                                    std::string keyword) {
  while (accounts != nullptr) {
    if (field == "ID" and accounts->id == (unsigned int)stoi(keyword) and
        accounts->locked == false) {
      return accounts;
    } else if (field == "FIRST" and accounts->fname == keyword and
               accounts->locked == false) {
      return accounts;
    } else if (field == "LAST" and accounts->lname == keyword and
               accounts->locked == false) {
      return accounts;
    } else if (field == "EMAIL" and accounts->email == keyword and
               accounts->locked == false) {
      return accounts;
    } else if (field == "PHONE" and accounts->phone == keyword and
               accounts->locked == false) {
      return accounts;
    }
    accounts = accounts->next;
  }
  return nullptr;
}

string ICS45C::BankManagementSystem::printDetails(Record *account) {
  if (account != nullptr) {
    if (account->locked == true) {
      return "------\nAccount #" + to_string(account->id) +
             "\n------\nSorry, this account has been locked.\n------\n";
    } else {
      stringstream stream;
      stream << fixed << setprecision(2) << account->balance;
      return "------\nAccount #" + to_string(account->id) +
             "\n------\nStatus: OPEN\nAccount holder: " + account->fname + " " +
             account->lname + "\nContact: " + account->phone + " / " +
             account->email + "\nBalance: " + stream.str() + "\n------\n";
    }
  } else {
    return "------\nAccount UNKNOWN\n------\nSorry, this account does not "
           "exist.\n------\n";
  }
}

string ICS45C::BankManagementSystem::printDetails(Record *accounts,
                                                  unsigned int id) {
  Record *account = helper_findaccount(accounts, id);
  return ICS45C::BankManagementSystem::printDetails(account);
}

void ICS45C::BankManagementSystem::updateAccount(
    Record *account, std::string firstName, std::string lastName,
    std::string phone, std::string email, bool locked) {
  if (account != nullptr) {
    account->fname = firstName;
    account->lname = lastName;
    account->email = email;
    account->phone = phone;
    account->locked = locked;
  }
}
void ICS45C::BankManagementSystem::updateAccount(
    Record *accounts, unsigned int id, std::string firstName,
    std::string lastName, std::string phone, std::string email, bool locked) {
  Record *account = helper_findaccount(accounts, id);
  ICS45C::BankManagementSystem::updateAccount(account, firstName, lastName,
                                              phone, email, locked);
}

double ICS45C::BankManagementSystem::deposit(Record *account, double value) {
  if ((account != nullptr) and (account->locked == false)) {
    if (value >= 0) {
      account->balance += value;
    }
  } else {
    return -123.45;
  }

  return account->balance;
}

double ICS45C::BankManagementSystem::deposit(Record *accounts, unsigned int id,
                                             double value) {
  Record *account = helper_findaccount(accounts, id);
  return ICS45C::BankManagementSystem::deposit(account, value);
}

double ICS45C::BankManagementSystem::withdrawal(Record *account, double value) {
  if ((account != nullptr) and (account->locked == false)) {
    if (value >= 0) {
      account->balance -= value;
    }
  } else {
    return -123.45;
  }
  return account->balance;
}

double ICS45C::BankManagementSystem::withdrawal(Record *accounts,
                                                unsigned int id, double value) {
  Record *account = helper_findaccount(accounts, id);
  return ICS45C::BankManagementSystem::withdrawal(account, value);
}

Record *ICS45C::BankManagementSystem::deleteAccount(Record *accounts,
                                                    unsigned int id) {
  Record *account, *head;
  account = helper_findaccount(accounts, id);
  head = accounts;
  if (account != nullptr) {
    while (accounts != nullptr) {
      if (accounts == account) {
        head = accounts->next;
        delete account;
        break;
      }
      if (accounts->next == account) {
        accounts->next = account->next;
        delete account;
        break;
      }
      accounts = accounts->next;
    }
  }
  return head;
}

unsigned int ICS45C::BankManagementSystem::countUnlockedAccounts(
    Record *accounts) {
  unsigned int unlocked_count = 0;
  while (accounts != nullptr) {
    if (accounts->locked == false) {
      unlocked_count += 1;
    }
    accounts = accounts->next;
  }
  return unlocked_count;
}

unsigned int ICS45C::BankManagementSystem::countLockedAccounts(
    Record *accounts) {
  unsigned int locked_count = 0;
  while (accounts != nullptr) {
    if (accounts->locked == true) {
      locked_count += 1;
    }
    accounts = accounts->next;
  }
  return locked_count;
}

unsigned int ICS45C::BankManagementSystem::countAllAccounts(Record *accounts) {
  return ICS45C::BankManagementSystem::countUnlockedAccounts(accounts) +
         ICS45C::BankManagementSystem::countLockedAccounts(accounts);
}

long double ICS45C::BankManagementSystem::getAverageBalance(Record *accounts) {
  unsigned int unlocked_count =
      ICS45C::BankManagementSystem::countUnlockedAccounts(accounts);
  long double total_balance = 0.0;
  while (accounts != nullptr) {
    if (accounts->locked == false) {
      total_balance += accounts->balance;
    }
    accounts = accounts->next;
  }
  if (unlocked_count > 0) {
    return total_balance / unlocked_count;
  } else {
    return 0.0;
  }
}

long double ICS45C::BankManagementSystem::getBankFunds(Record *accounts) {
  long double total_balance = 0.0;
  while (accounts != nullptr) {
    if (accounts->locked == false) {
      total_balance += accounts->balance;
    }
    accounts = accounts->next;
  }
  return total_balance;
}

void ICS45C::BankManagementSystem::closeBank(Record *accounts) {
  Record *next;
  while (accounts != nullptr) {
    next = accounts->next;
    delete accounts;
    accounts = next;
  }
}