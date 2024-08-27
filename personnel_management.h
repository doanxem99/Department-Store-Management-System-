// Copyright [2024] <Your name>
#ifndef PERSONNEL_MANAGEMENT_H_
#define PERSONNEL_MANAGEMENT_H_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "./asset.h"

const int MAX_PERSONNELS = 50;  // Assuming that the number of personnels won't exceed 50
const int ID_W = 5;
const int NAME_W = 20;
const int DEPT_W = 15;
const int RANK_W = 10;
const int POS_W = 30;
const int TASK_W = 70;

class PersonnelManagement {
 private:
    static bool ID_in_use[MAX_PERSONNELS];
    
    int first_unused_ID();      // "Generate" ID for new personnel
    void importPersonnel();
    void exportPersonnel();

    void addPersonnel(std::string rank);
    bool removePersonnel(std::string rank);
    bool updatePersonnel(std::string rank);
    void listPersonnel(std::string rank);  // List all personnel in the system

    bool searchPersonnel(std::string rank); // Search for a personnel by name, ID, etc.
                                            // by using Hash Table or Binary Search Tree

    void assignTask(std::string rank);      // Assign task to personnel
    void listTask();                        // List all tasks in the system

    // void reportByUser(int ID);           // Report by user when they want to
                                            // change salary, work hour, etc.

 public:
    friend class ExpenseManagement;
    struct Personnel {
        int ID;
        std::string name;
        std::string address;
        std::string phone;
        std::string email;
        std::string department;
        std::string rank;       // including "staff" < "manager" < "owner"
        std::string position;   // for example: janitor, cashier, security,...
        std::string task;       // for example: cleaning warehouse, nighttime security, daytime security...
        int salary;
        std::string currency;   // default is USD

        bool operator > (const Personnel &p)
        {
            return (ID > p.ID);
        }
        
        bool operator < (const Personnel &p)
        {
            return (ID < p.ID);
        }

        bool operator == (const Personnel &p)
        {
            return (ID == p.ID);
        }
        
        friend std::ostream& operator << (std::ostream& os, const Personnel& p)
        {
            os << std::left << std::setfill(' ');
            os << std::setw(ID_W) << p.ID << std::setw(NAME_W) << p.name << std::setw(DEPT_W) << p.department
                << std::setw(RANK_W) << p.rank << std::setw(POS_W) << p.position << std::setw(TASK_W) << p.task;
            return os;
        }
    };

    PersonnelManagement();
    ~PersonnelManagement();

    // Step-by-step for personnel management when user choose this option
    void stepbystepForPersonnelManagement(int ID, std::string rank);

    // calculate salary is decided by expense management
};


#endif  // PERSONNEL_MANAGEMENT_H_
