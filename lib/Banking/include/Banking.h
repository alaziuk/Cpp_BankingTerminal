/*!
    @author Adam Łaziuk (alaziuk)
*/


#ifndef Banking
#define Banking

#include <unordered_map>

#define NUMBER_OF_TRIES 3

enum Interface {
    loginMenu,
    accountLogin,
    accountSignin,
    exitApp,
    mainMenu,
    moneyDeposit,
    moneyWithdrawal,
    moneyTransfer,
    accountLogout
};

struct User {
    std::string username;
    std::string password;
    double balance;
    bool exit = false;
};

class Account {
    public:
        User userData_;
        std::unordered_map<std::string, std::pair<std::string, double>> users;
        std::pair<Interface, User> logout(Interface whereTo);    
        bool dataSynced_;
        bool goBack;
        void deposit();
        void withdrawal();
        void transfer();    
};

class Database {
    public:
        Database();
        std::unordered_map<std::string, std::pair<std::string, double>> users;
        void saveUsersToFile();
    private:
        bool loadUsersFromFile();
};


class Bank {
    public:
        Bank(Database &db, Account &acc);
        void App();
    private:
        Account &account;
        Database &database;
        User userData_;
        bool running_ = true;
        Interface currentInterface_ = loginMenu;
        void rewriteData();
        void chooseInterface();
        void menu();
        void login();
        void signin();
        void userMenu();
        void transferToDatabase(std::pair <Interface, User>instance);
        void sync();
};
#endif /* Banking */