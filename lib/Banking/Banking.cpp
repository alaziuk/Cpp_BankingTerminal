/*!
    @author Adam Łaziuk (alaziuk)
*/

#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include "include/Banking.h"

const int numberOfTries = NUMBER_OF_TRIES - 1;

/*!
    @brief  Initializes Bank class
*/

Bank::Bank(Database &db, Account &acc) : database(db), account(acc) {}

/*!
    @brief  Initializes Banking App asking user for login or sign in
*/

void Bank::App() {
    while (running_) {
        chooseInterface();
    }
} /* App */

/*!
    @brief  Interface mode
*/

void Bank::chooseInterface() {
    switch (currentInterface_) {
        case loginMenu:
            menu();
            break;
        case accountLogin:
            login();
            break;
        case accountSignin:
            signin();
            break;
        case exitApp:
            running_ = false;
            rewriteData();
            return;
        case mainMenu:
            userMenu();
            break;
        case moneyDeposit:
            account.deposit();
            if (account.goBack) {
                currentInterface_ = mainMenu;
                account.goBack = false;
            }
            sync();
            break;
        case moneyWithdrawal:
            account.withdrawal();
            if (account.userData_.balance == 0) {
                currentInterface_ = mainMenu;
            }
            if (account.goBack) {
                currentInterface_ = mainMenu;
                account.goBack = false;
            }
            sync();
            break;
        case moneyTransfer:
            account.transfer();
            if (account.userData_.balance == 0) {
                currentInterface_ = mainMenu;
            }
            if (account.goBack) {
                currentInterface_ = mainMenu;
                account.goBack = false;
            }
            sync();
            break;
        case accountLogout:
            if (userData_.exit){
                transferToDatabase(account.logout(exitApp));
            } else {
                transferToDatabase(account.logout(loginMenu));
            }
            break;
        default:
            break;
    } /* switch interface */
} /* interfaces */

/*!
    @brief  Initial logging in menu
*/

void Bank::menu() {
    while (true) {
        std::cout << "*********************************************\n";
        std::cout << "Welcom to my Banking App!\n";
        std::cout << "What would you like to do?\n";
        std::cout << "1. Login to an account\n";
        std::cout << "2. Create an account\n";
        std::cout << "3. Exit\n";
        std::cout << "*********************************************\n";
        
        int choice, tries = 0;
        while (!(std::cin >> choice)) {
            std::cout << "That is not an integer!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            tries++;
            if (tries > numberOfTries) {break;}
        } /* while (!input) */

        if (tries > numberOfTries) {continue;}

        switch (choice) {
            case 1:
                currentInterface_ = accountLogin;
                return;
            case 2:
                currentInterface_ = accountSignin;
                return;
            case 3:
                currentInterface_ = exitApp;
                return;
            default:
                std::cout << "Invalid choice, try again\n";
                break;
        } /* switch (choice) */
    } /* while (true) */
} /* menu */

/*!
    @brief  Login into banking account
*/


void Bank::login() {
    currentInterface_ = loginMenu;
    int tries = 0;
    std::string username, password;
    while (true) {
        if (tries > numberOfTries) {
            std::cout << "You have achieved limit for login tries!\n";
            break;
        } /* if tries */
        std::cout << "*********************************************\n";
        std::cout << "You have entered logging in page\n";
        std::cout << "Enter usename: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        auto iterator = database.users.find(username);
        if (iterator == database.users.end()) {
            std::cout << "Wrong credentials!\n";
            std::cout << "*********************************************\n";
            tries++;
            continue;
        } /* if !username */
        if (iterator->second.first != password) {
            std::cout << "Wrong credentials!\n";
            std::cout << "*********************************************\n";
            tries++;
            continue;
        } /* if !password */
        userData_.username = iterator->first;
        userData_.password = iterator->second.first;
        userData_.balance = iterator->second.second;
        std::cout << "Succesfully logged in\n";
        std::cout << "*********************************************\n";
        currentInterface_ = mainMenu;
        break;
    } /* while true */
} /* login */

/*!
    @brief  Singing in into banking account
*/

void Bank::signin() {
    currentInterface_ = loginMenu;
    int tries = 0;
    std::string username, password;
    while (true) {
        if (tries > numberOfTries) {
            std::cout << "You have achieved limit for signin tries!\n";
            break;
        } /* if tries */
        std::cout << "*********************************************\n";
        std::cout << "You have entered signing in page\n";
        std::cout << "Enter usename: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;
        auto iterator = database.users.find(username);
        if (iterator != database.users.end()) {
            std::cout << "Username already exists!\n";
            tries++;
            continue;
        }
        std::pair <std::string, double> tPair(password, 0.0);
        database.users[username] = tPair;
        database.saveUsersToFile();
        break;
        std::cout << "You have signed in\n";
        currentInterface_ = mainMenu;
    } /* while true */
    std::cout << "*********************************************\n";
    
} /* signin */

/*!
    @brief Updates database
*/

void Bank::rewriteData() {
    std::ifstream inputFile("users.txt");
    std::ofstream outputFile("temp.txt");

    std::string line;

    while (getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string fileUsername, filePassword;
        double fileBalance;

        if (iss >> fileUsername >> filePassword >> fileBalance) {
            if (fileUsername == userData_.username) {
                outputFile << userData_.username << " " << userData_.password << " " << userData_.balance << std::endl;
            } else {
                outputFile << line << std::endl;
            } /* if =username */
        } /* if is line */
    } /* while line */

    inputFile.close();
    outputFile.close();

    remove("users.txt");
    rename("temp.txt", "users.txt");
} /* backupdata */

/*!
    @brief User menu
*/

void Bank::userMenu() {
    if (!account.dataSynced_) {
        account.userData_ = userData_;
        account.users = database.users;
        account.dataSynced_ = true;
    } /* if data synced */

   while (true) {
        std::cout << "*********************************************\n";
        std::cout << "Welcome, " << account.userData_.username << "!\n";
        std::cout << "Your account balance is " << account.userData_.balance << " zl\n";
        std::cout << "What would you like to do?\n";
        std::cout << "1. Deposit\n";
        std::cout << "2. Withdrawal\n";
        std::cout << "3. Transfer\n";
        std::cout << "4. Log out\n";
        std::cout << "5. Exit\n";
        std::cout << "*********************************************\n";
        
        int choice, tries = 0;
        while (!(std::cin >> choice)) {
            std::cout << "That is not an integer!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            tries++;
            if (tries > numberOfTries) {break;}
        } /* while (!input) */

        if (tries > numberOfTries) {continue;}

        switch (choice) {
            case 1:
                currentInterface_ = moneyDeposit;
                return;
            case 2:
                currentInterface_ = moneyWithdrawal;
                return;
            case 3:
                currentInterface_ = moneyTransfer;
                return;
            case 4:
                userData_.exit = false;
                currentInterface_ = accountLogout;
                return;
            case 5:
                userData_.exit = true;
                currentInterface_ = accountLogout;
                return;
            default:
                std::cout << "Invalid choice, try again\n";
                break;
        } /* switch (choice) */
    } /* while (true) */
} /* userMenu*/

void Bank::transferToDatabase(std::pair <Interface, User>instance) {
    currentInterface_ = instance.first;
    userData_ = instance.second;
    database.saveUsersToFile();
}

void Bank::sync() {
    userData_ = account.userData_;
    database.users = account.users;
    database.saveUsersToFile();
}

/*!
    @brief Database constructor - loads users from users.txt file
*/

Database::Database() {
    loadUsersFromFile();
}

/*!
    @brief Saves users to users.txt file
*/

void Database::saveUsersToFile() {
    std::ofstream userFile("users.txt");

    for (const auto &user : users) {
        userFile << user.first << " " << user.second.first << " " << user.second.second << std::endl;
    }
    userFile.close();
} /* saveUsersToFile */

/*!
    @brief Loads users from users.txt file
*/

bool Database::loadUsersFromFile() {
    std::ifstream userFile("users.txt");
    if (!userFile) {return false;}
    std::string line;
    while (getline(userFile, line)) {
        std::istringstream iss(line);
        std::string username, password;
        double balance;
        if (iss >> username >> password >> balance) {
            std::pair<std::string, double> tPair(password, balance);
            users[username] = tPair;
        } /* if is line */
    } /* while line */
    userFile.close();
    return true;
} /* loadusersfromfile */

/*!
    @brief Account logout
*/

std::pair<Interface, User>Account::logout(Interface whereTo) {
    dataSynced_ = false;
    User userInstance = userData_; 
    return std::make_pair(whereTo, userInstance);
}

/*!
    @brief Money deposit
*/

void Account::deposit() {
    double amount = 0;
    while (true) {
        std::cout << "*********************************************\n";
        std::cout << "Your have entered deposit page\n";
        std::cout << "What would you like to do?\n";
        std::cout << "1. Enter amount\n";
        std::cout << "2. Go back\n";
        int choice,tries = 0;
        while (!(std::cin >> choice)) {
            std::cout << "That is not a integer!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            tries++;
            if (tries > numberOfTries) {break;}
        } /* while (!input) */

        if (amount < 0) {
            std::cout << "Amount cannot be negative!\n";
            amount = 0;
            break;
        }

        if (tries > numberOfTries) {continue;}

        switch (choice) {
            case 1:
                std::cout << "How much would you like to deposit??\n";
                while (!(std::cin >> amount)) {
                    std::cout << "That is not a double!\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    tries++;
                    if (tries > numberOfTries) {break;}
                } /* while (!input) */

                if (tries > numberOfTries) {continue;}
                break;
            case 2:
                goBack = true;
                return;
            default:
                std::cout << "Invalid choice, try again\n";
                break;
        }
        break;
    } /* while (true) */
    userData_.balance += amount;
    std::cout << "You have deposited " << amount << " to your account\n";
    std::cout << "*********************************************\n";
    goBack = true;
}

/*!
    @brief Money withdrawal
*/

void Account::withdrawal() {
    if (userData_.balance == 0){
        std::cout << "You don't have any funds to withraw!\n";
        return;
    }

    double amount = 0;
    while (true) {
        std::cout << "*********************************************\n";
        std::cout << "Your account balance is " << userData_.balance << " zl\n";
        std::cout << "What would you like to do?\n";
        std::cout << "1. Enter amount\n";
        std::cout << "2. Go back\n";
        int choice,tries = 0;
        while (!(std::cin >> choice)) {
            std::cout << "That is not a integer!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            tries++;
            if (tries > numberOfTries) {break;}
        } /* while (!input) */

        if (tries > numberOfTries) {continue;}

        switch (choice) {
            case 1:
                std::cout << "How much would you like to withdraw??\n";
                while (!(std::cin >> amount)) {
                    std::cout << "That is not a double!\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    tries++;
                    if (tries > numberOfTries) {break;}
                } /* while (!input) */
                
                if (amount < 0) {
                    std::cout << "Amount cannot be negative!\n";
                    amount = 0;
                    break;
                }

                if (tries > numberOfTries) {continue;}

                if (amount > userData_.balance) {
                    std::cout << "You don't have enough balance!\n";
                    continue;
                }
                break;
            case 2:
                goBack = true;
                return;
            default:
                std::cout << "Invalid choice, try again\n";
                break;
        }
        break;
    } /* while (true) */
    userData_.balance -= amount;
    std::cout << "You have withrawn " << amount << " from your account\n";
    std::cout << "*********************************************\n";
    goBack = true;
}

/*!
    @brief Money transfer
*/

void Account::transfer() {
    if (userData_.balance == 0){
        std::cout << "You don't have any funds to withraw!\n";
        return;
    }

    double amount = 0;
    std::string user = "None";
    while (true) {
        std::cout << "*********************************************\n";
        std::cout << "Your account balance is " << userData_.balance << " zl\n";
        std::cout << "Current transfer to " << user << '\n';
        std::cout << "Current amount to transfer to: " << amount << '\n';
        std::cout << "What would you like to do?\n";
        std::cout << "1. Enter user to transfer to\n";
        std::cout << "2. Enter amount\n";
        std::cout << "3. Proceed with transfer\n";
        std::cout << "4. Go back\n";
        int choice,tries = 0;
        while (!(std::cin >> choice)) {
            std::cout << "That is not a integer!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            tries++;
            if (tries > numberOfTries) {break;}
        } /* while (!input) */

        if (tries > numberOfTries) {continue;}

        switch (choice) {
            case 1: {
                std::cout << "To whom would you like to transfer money to?\n";
                while (!(std::cin >> user)) {
                    std::cout << "That is not a proper string!\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    tries++;
                    if (tries > numberOfTries) {break;}
                } /* while (!input) */

                if (tries > numberOfTries) {break;}

                
                auto iterator = users.find(user);
                if (iterator == users.end()) {
                    std::cout << "Username don't exist!\n";
                    user = "None";
                    tries++;
                    break;
                }
                break;
            }
            case 2: {
                std::cout << "How much would you like to transfer?\n";
                while (!(std::cin >> amount)) {
                    std::cout << "That is not a double!\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    tries++;
                    if (tries > numberOfTries) {break;}
                } /* while (!input) */

                if (amount < 0) {
                    std::cout << "Amount cannot be negative!\n";
                    amount = 0;
                    break;
                }

                if (tries > numberOfTries) {break;}

                if (amount > userData_.balance) {
                    std::cout << "You don't have enough balance!\n";
                    amount = 0;
                    break;
                }
                break;
            }
            case 3: {
                goBack = true;
                users[userData_.username].second -= amount;
                userData_.balance -= amount;
                users[user].second += amount;
                return;
            }
            case 4: {
                goBack = true;
                return;
            }
            default: {
                std::cout << "Invalid choice, try again\n";
                break;
            }
        }
    } /* while (true) */


}