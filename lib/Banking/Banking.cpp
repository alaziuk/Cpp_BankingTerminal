/*!
    @author Adam Łaziuk (alaziuk)
*/

#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include "Banking.h"

const int numberOfTries = NUMBER_OF_TRIES - 1;

/*!
    @brief  Initializes Bank class
*/

Bank::Bank(Database &db) : database(db) {}

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
            backupData();
            return;
        case mainMenu:
            userMenu();
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
        userData_.username = username;
        userData_.password = password;
        userData_.balance = iterator->second.second;
        std::cout << "Succesfully logged in\n";
        std::cout << "*********************************************\n";
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
    } /* while true */
    std::cout << "*********************************************\n";
    
} /* signin */

/*!
    @brief Updates database
*/

void Bank::backupData() {
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
    Account account(userData_);

} /* userMenu*/

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
}

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
    @brief Account constructor
*/

Account::Account(User &user) : userData_(user) {}