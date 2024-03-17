#include <iostream>
#include <Banking.h>

int main () {
    Database database;
    Account account;
    Bank bank(database, account);

    bank.App();
    return 0;
}