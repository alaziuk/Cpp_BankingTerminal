/*!
    @author Adam Łaziuk (alaziuk)
*/


#ifndef Banking
#define Banking

class Bank {
    public:
        void BankingApp();
    private:
        void login();
        void signin();
};

class Database {
    public:
        Database();
    private:
        void loadUsersFromFile();
};

#endif /* Banking */