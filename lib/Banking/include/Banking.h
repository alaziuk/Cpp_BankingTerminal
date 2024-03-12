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
    mainMenu
};

struct User {
    std::string username;
    std::string password;
    double balance;
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
        Bank(Database &db);
        void App();
    private:
        Database &database;
        User userData_;
        bool running_ = true;
        Interface currentInterface_ = loginMenu;
        void backupData();
        void chooseInterface();
        void menu();
        void login();
        void signin();
        void userMenu();
};

class Account {
    public:
        Account(User &user);
        void mainMenu();
    private:
        User userData_;
};
#endif /* Banking */