// Copyright: [2024] Nguyen Dinh Manh
#include "./department_store.h"

void login() {
    std::cout << "Please enter your username: ";
    std::string username;
    std::cin >> username;
    std::cout << "Please enter your password: ";
    std::string password;
    std::cin >> password;
    // check if the username and password are correct
    // if correct, display the main menu
    // if not, display an error message
}
void signup() {
    std::cout << "Please enter your username: ";
    std::string username;
    std::cin >> username;
    std::cout << "Please enter your password: ";
    std::string password;
    std::cin >> password;
    // add the new user to the database
    // display a success message
}
int main() {
    std::cout << "Welcome to the Department Store Management System!"
                << std::endl;
    std::cout << "Please select an option:" << std::endl;
    std::cout << "1. Login" << std::endl;
    std::cout << "2. Sign up" << std::endl;
    std::cout << "3. Exit" << std::endl;
    int choice;
    std::cin >> choice;
    switch (choice) {
        case 1:
            login();
            break;
        case 2:
            signup();
            break;
        case 3:
            std::cout << "Goodbye! See you later" << std::endl;
            break;
        default:
            std::cout << "Invalid choice" << std::endl;
            break;
    }
}
