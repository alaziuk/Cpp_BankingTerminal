/*!
    @author Adam Łaziuk (alaziuk)
*/

#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include "Banking.h"


/*!
    @brief  Initializes Bank class
*/

Bank::Bank(Database &db) : database(db) {

}

/*!
    @brief  Initializes Banking App asking user for login or sign in
*/

void Bank::App() {
    while (running_) {
        chooseInterface();
    }
}

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
            return;
        default:
            break;
    } /* switch interface */
}

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
            if (tries > 3) {break;}
        } /* while (!input) */

        if (tries > 3) {continue;}

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
}

/*!
    @brief  Login into banking account
*/


void Bank::login() {
    currentInterface_ = loginMenu;
    int tries = 0;
    std::string username, password;
    while (true) {
        if (tries > 3) {
            std::cout << "You have achieved limit for login tries!\n";
            break;
        }
        std::cout << "*********************************************\n";
        std::cout << "You have entered logging in page\n";
        std::cout << "Enter usename: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;
        std::cout << "*********************************************\n";

        auto iterator = database.users.find(username);
        if (iterator == database.users.end()) {
            std::cout << "Wrong credentials!";
            tries++;
            continue;
        }
        if (iterator->second != password) {
            std::cout << "Wrong credentials!";
            tries++;
            continue;
        }
        userData_.username = username;
        userData_.password = password;
        userData_.balance = database.userBalance.find(username)->second;
        std::cout << "Succesfully logged in\n";
        break;
    }
}

/*!
    @brief  Singing in into banking account
*/

void Bank::signin() {
    std::cout << "You have entered signing in page\n";
    currentInterface_ = loginMenu;
}

/*!
    @brief Database constructor - loads users from users.txt file
*/

Database::Database() {
    loadUsersFromFile();
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
            users[username] = password;
            userBalance[username] = balance;
        }
    }
    return true;
}