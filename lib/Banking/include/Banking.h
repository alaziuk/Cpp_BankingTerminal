/*!
    @author Adam Łaziuk (alaziuk)
*/


#ifndef Banking
#define Banking

#include <unordered_map>

enum Interface {
    loginMenu,
    accountLogin,
    accountSignin,
    exitApp
};

struct User {
    std::string username;
    std::string password;
    double balance;
};

class Database {
    public:
        Database();
        std::unordered_map<std::string, std::string> users;
        std::unordered_map<std::string, double> userBalance;
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
        void chooseInterface();
        void menu();
        void login();
        void signin();
};
#endif /* Banking */