/*!
    @author Adam Łaziuk (alaziuk)
*/

#include <iostream>
#include <fstream>
#include <limits>
#include "Banking.h"

/*!
    @brief  Initializes Banking App asking user for login or sign in
*/

void Bank::BankingApp() { 
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
                login();
                break;
            case 2:
                signin();
                break;
            case 3:
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
    std::cout << "You havc entered logging in page\n";
}

/*!
    @brief  Singing in into banking account
*/

void Bank::signin() {
    std::cout << "You havc entered signing in page\n";
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

void Database::loadUsersFromFile() {

}