#ifndef DEPARTMENT_STORE_H_
#define DEPARTMENT_STORE_H_

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <array>

#include "./asset.h"
#include "./product_management.h"
#include "./expense_management.h"
#include "./personnel_management.h"

class DepartmentStore {
private:
    struct User {
        std::string account;
        std::string password;
        int ID;
        std::string rank;
        std::string name;
        User() {
            account = "";
            password = "";
            ID = 0;
            rank = "";
            name = "";
        }
        bool operator==(const User &other) {
            return account == other.account && password == other.password;
        }
    };
    std::vector<User> userList;
    ProductManagement productManagement;
    ExpenseManagement expenseManagement;
    PersonnelManagement personnelManagement;
    void drawDepartmentStore_open();
    bool logIn();
    short signUp();
    bool changePassword();
    bool checkFormatAccount(const std::string &account);
    bool checkFormatPassword(const std::string &password);

    friend class Asset;
 public:
    void drawDepartmentStore_close(std::string title);
    User currentUser;
    void loadAccount();
    void exportAccount();
    bool run();
    DepartmentStore() {
        loadAccount();
    }
    ~DepartmentStore() {
        exportAccount();
        drawDepartmentStore_close("Goodbye " + currentUser.name + "!");
    }
};

#endif  // DEPARTMENT_STORE_H_
