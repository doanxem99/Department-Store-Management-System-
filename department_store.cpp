#include "./department_store.h"


void DepartmentStore::drawDepartmentStore_close(std::string title) {
    std::cout << "            +-------------------------------------+" << std::endl;
    std::cout << "            |          DEPARTMENT STORE           |" << std::endl;
    std::cout << "  +---------+-------------------------------------+---------+" << std::endl;
    std::cout << "  |       | |";
    // Size of title is 37
    int size = title.size();
    for (int i = 0; i < 37 / 2 - size / 2; i++) {
        title = " " + title;
    }
    while (title.size() < 37) {
        title += " ";
    }
    std::cout << title << "| |       |" << std::endl;
    std::cout << "  |       | |-------------------------------------| |       |" << std::endl;
    std::cout << "  |       | |                                     | |       |" << std::endl;
    std::cout << "  |       | |                                     | |       |" << std::endl;
    std::cout << "  |       | |            +---------+              | |       |" << std::endl;
    std::cout << "  |       | |            |    |    |              | |       |" << std::endl;
    std::cout << "  |       | |            |    |    |              | |       |" << std::endl;
    std::cout << "  |       | |            |  . | .  |              | |       |" << std::endl;
    std::cout << "  |       | |            |    |    |              | |       |" << std::endl;
    std::cout << "  |       | |            |    |    |              | |       |" << std::endl;
    std::cout << "  |       | |            +---------+              | |       |" << std::endl;
    std::cout << "  +-------+-+-------------------------------------+-+-------+" << std::endl;
    std::cout << "                     |_________________|                     " << std::endl;
    std::cout << "                    |___________________|                    " << std::endl;
    std::cout << "                   |_____________________|                   " << std::endl; 
}
void DepartmentStore::drawDepartmentStore_open() {
    std::string title = "Hello " + currentUser.name + "!";
    std::cout << "            +-------------------------------------+" << std::endl;
    std::cout << "            |          DEPARTMENT STORE           |" << std::endl;
    std::cout << "  +---------+-------------------------------------+---------+" << std::endl;
    std::cout << "  |       | |";
    // Size of title is 37
    int size = title.size();
    for (int i = 0; i < 37 / 2 - size / 2; i++) {
        title = " " + title;
    }
    while (title.size() < 37) {
        title += " ";
    }
    std::cout << title << "| |       |" << std::endl;
    std::cout << "  |       | |-------------------------------------| |       |" << std::endl;
    std::cout << "  |       | |                                     | |       |" << std::endl;
    std::cout << "  |       | |                                     | |       |" << std::endl;
    std::cout << "  |       | |            +---------+              | |       |" << std::endl;
    std::cout << "  |       | |         /  |         | \\            | |       |" << std::endl;
    std::cout << "  |       | |        |   |         |   |          | |       |" << std::endl;
    std::cout << "  |       | |        |   |         |   |          | |       |" << std::endl;
    std::cout << "  |       | |        |.  |         |  .|          | |       |" << std::endl;
    std::cout << "  |       | |        |   |         |   |          | |       |" << std::endl;
    std::cout << "  |       | |        |  /           \\  |          | |       |" << std::endl;
    std::cout << "  +-------+-+-------------------------------------+-+-------+" << std::endl;
    std::cout << "                     |_________________|                     " << std::endl;
    std::cout << "                    |___________________|                    " << std::endl;
    std::cout << "                   |_____________________|                   " << std::endl; 
}

bool DepartmentStore::logIn() {
    User newUser;
    std::cout << "Please enter your username: ";
    std::cin >> newUser.account;
    std::cout << "Please enter your password: ";
    std::cin >> newUser.password;
    std::cout << "-------------------" << std::endl;
    for (User user : userList) {
        if (newUser == user) {
            currentUser = user;
            return true;
        }
    }
    return false;
}
bool DepartmentStore::checkFormatAccount(const std::string &account) {
    if (account.size() < 6 || account.size() > 20) {
        return false;
    }
    for (char c : account) {
        if (!isalnum(c)) {
            return false;
        }
    }
    return true;
}
bool DepartmentStore::checkFormatPassword(const std::string &password) {
    if (password.size() < 6 || password.size() > 20) {
        return false;
    }
    bool hasDigit = false;
    bool hasLetter = false;
    bool hasSpecial = false;
    for (char c : password) {
        if (isdigit(c)) {
            hasDigit = true;
        }
        if (isalpha(c)) {
            hasLetter = true;
        }
        if (!isalnum(c)) {
            hasSpecial = true;
        }
    }
    return hasDigit && hasLetter && hasSpecial;
}
short DepartmentStore::signUp() {
    User newUser;
    std::cout << "Please enter your username: ";
    std::cin >> newUser.account;
    std::cout << "Please enter your password: ";
    std::cin >> newUser.password;
    std::cout << "Please confirm your password: ";
    std::string confirmPassword;
    std::cin >> confirmPassword;
    if (!checkFormatAccount(newUser.account)) {
        return 3;
    }
    if (!checkFormatPassword(newUser.password)) {
        return 4;
    }
    std::cout << "-------------------" << std::endl;
    if (newUser.password != confirmPassword) {
        return 1;
    }
    for (User user : userList) {
        if (newUser.account == user.account) {
            return 2;
        }
    }
    newUser.ID = userList.size() + 1;
    userList.push_back(newUser);
    currentUser = newUser;
    return 0;
}
bool DepartmentStore::changePassword() {
    std::string newPassword;
    std::cout << "Please enter your new password: ";
    std::cin >> newPassword;
    std::cout << "Please confirm your new password: ";
    std::string confirmPassword;
    std::cin >> confirmPassword;
    if (!checkFormatPassword(newPassword)) {
        std::cout << "Your password must be between 6 and 20 characters long. ";
        std::cout << "It must contain at least one letter, one number, and one special character. ";
        std::cout << "Please try again." << std::endl;
        return true;
    }
    if (newPassword != confirmPassword) {
        std::cout << "Your password and confirm password do not match. ";
        std::cout << "Please try again." << std::endl;
        return true;
    }
    for (User &user : userList) {
        if (currentUser == user) {
            user.password = newPassword;
            break;
        }
    }
    std::cout << "Your password has been changed successfully!" << std::endl;
    return false;
}
void DepartmentStore::loadAccount() {
    // Load account and password
    std::ifstream input("secret.txt");
    while (!input.eof()) {
        User user;
        input >> user.account >> user.password >> user.ID;
        if (user.account == "") {
            break;
        }
        userList.push_back(user);
    }
    input.close();
}
void DepartmentStore::exportAccount() {
    // Add or remove account
    if (Asset::changePersonnel.size() > 0) {
        for (auto &p : Asset::changePersonnel) 
            if (p.first == "add") {
                User user;
                user.account = p.second.name;
                user.password = p.second.name + std::to_string(p.second.ID);
                user.ID = p.second.ID;
                userList.push_back(user);
            }
            else if (p.first == "remove") {
                for (int i = 0; i < userList.size(); i++) {
                    if (userList[i].ID == p.second.ID) {
                        userList.erase(userList.begin() + i);
                        break;
                    }
                }
            }
    }
    // Export account and password
    std::ofstream output("secret.txt");
    for (User user : userList) {
        output << user.account << " " << user.password << " " << user.ID << std::endl;
    }
    output.close();
}
bool DepartmentStore::run() {
    drawDepartmentStore_close("Welcome !");
    std::cout << "Welcome to the Department Store Management System!"
                << std::endl;
    std::cout << "Please select an option:" << std::endl;
    while (true) {
        std::cout << "1. Login" << std::endl;
        std::cout << "2. Sign up" << std::endl;
        std::cout << "3. Continue as a guest" << std::endl;
        std::cout << "4. Exit" << std::endl;
        int choice;
        std::cout << std::endl << "Your choice: ";
        std::cin >> choice;
        std::cout << "-------------------" << std::endl;
        switch (choice) {
            case 1:
                while (!logIn()) {
                    std::cout << "Your username or password is incorrect. ";
                    std::cout << "Please try again." << std::endl;
                    std::cout << "1. Continue to login" << std::endl;
                    std::cout << "2. Go back" << std::endl;
                    std::cout << std::endl << "Your choice: ";
                    std::cin >> choice;
                    std::cout << "-------------------" << std::endl;
                    if (choice == 2) {
                        break;
                    }
                }
                break;
            case 2:
                short error;
                error = signUp();
                while (error != 0) {
                    switch (error) {
                        case 1:
                            std::cout << "Your password and confirm password do not match. ";
                            break;
                        case 2:
                            std::cout << "This username is already taken. ";
                            break;
                        case 3:
                            std::cout << "Your account must be between 6 and 20 characters long. ";
                            std::cout << "It must contain only letters and numbers. ";
                            break;
                        case 4:
                            std::cout << "Your password must be between 6 and 20 characters long. ";
                            std::cout << "It must contain at least one letter, one number, and one special character. ";
                            break;
                    }
                    std::cout << "Please try again." << std::endl;
                    std::cout << "1. Continue to sign up" << std::endl;
                    std::cout << "2. Go back" << std::endl;
                    std::cout << std::endl << "Your choice: ";
                    std::cin >> choice;
                    std::cout << "-------------------" << std::endl;
                    if (choice == 2) {
                        break;
                    }
                }
                break;
            case 3:
                currentUser.account = "guest";
                break;
            case 4:
                return false;
                break;
            default:
                std::cout << "Invalid choice" << std::endl;
                break;
        }
        if (currentUser.account != "") {
            break;
        }
    }
    if (currentUser.account != "") {
        PersonnelManagement::Personnel target;
        if (currentUser.account == "guest") {
            target.rank = "staff";
            target.name = "guest";
        } else {
            target.ID = currentUser.ID;
            Asset::personnelTree.find(target);
            currentUser.rank = target.rank;
            currentUser.name = target.name;
        }

        drawDepartmentStore_open();
        std::cout << "Welcome " << currentUser.account << "!" << std::endl;
        std::cout << "Please select an option:" << std::endl;
        while (true) {
            std::cout <<"Account: " << currentUser.account << std::endl;
            std::cout <<"ID: " << currentUser.ID << std::endl;
            std::cout << "1. Product Management" << std::endl;
            std::cout << "2. Personnel Management" << std::endl;
            std::cout << "3. Expense Management" << std::endl;
            std::cout << "4. Change password" << std::endl;
            std::cout << "5. Sign out" << std::endl;
            std::cout << "6. Exit" << std::endl;
            int choice;
            std::cout << std::endl << "Your choice: ";
            std::cin >> choice;
            std::cout << "-------------------" << std::endl;
            switch (choice) {
                case 1:
                    productManagement.stepbystepForProductManagement();
                    break;
                case 2:
                    personnelManagement.stepbystepForPersonnelManagement(currentUser.ID, currentUser.rank);
                    break;
                case 3:
                    // expenseManagement.stepbystepForExpenseManagement();
                    break;
                case 4:
                    while (changePassword()) {
                        std::cout << "1. Continue to change password" << std::endl;
                        std::cout << "2. Go back" << std::endl;
                        std::cout << std::endl << "Your choice: ";
                        std::cin >> choice;
                        std::cout << "-------------------" << std::endl;
                        if (choice == 2) {
                            break;
                        }
                    }
                case 5:
                    drawDepartmentStore_close("Goodbye " + currentUser.name + "!");
                    currentUser = User();
                    break;
                case 6:
                    std::cout << "Goodbye! See you later" << std::endl;
                    break;
                default:
                    std::cout << "Invalid choice" << std::endl;
                    break;
            }
            if (choice == 5) {
                return true;
            }
            if (choice == 6) {
                return false;
            }
        }
    }
    return false;
}
int main() {
    DepartmentStore departmentStore;
    try {
        while (true) {
            if (!departmentStore.run()) {
                break;
            }
        }
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
