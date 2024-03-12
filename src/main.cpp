#include <iostream>
#include <Banking.h>

int main () {
    Database database;
    Bank bank(database);

    bank.App();
    return 0;
}