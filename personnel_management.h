// Copyright [2024] <Your name>
#ifndef PERSONNEL_MANAGEMENT_H_
#define PERSONNEL_MANAGEMENT_H_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "./asset.h"
#include "./ds.h"

const int id_w = 5;
const int name_w = 20;
const int dept_w = 15;
const int rank_w = 10;
const int pos_w = 30;
const int task_w = 80;

class PersonnelManagement {
 private:
    static bool ID_in_use[50];  // Assuming that the number of personnels won't exceed 50
    
    int first_unused_ID();      // "Generate" ID for new personnel
    void importPersonnel();
    void exportPersonnel();

    // input of these functions should be the rank
    void addPersonnel(std::string rank);
    bool removePersonnel(std::string rank);
    bool updatePersonnel(std::string rank);
    void printPersonnel(std::string rank);  // List all personnel in the system

    bool searchPersonnel(std::string rank); // Search for a personnel by name, ID, etc.
                                                // by using Hash Table or Binary Search Tree

    void assignTask(std::string rank);      // Assign task to personnel
    void listTask();                            // List all tasks in the system

    // void reportByUser(int ID);               // Report by user when they want to
                                                // change salary, work hour, etc.

 public:
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
            os << std::setw(id_w) << p.ID << std::setw(name_w) << p.name << std::setw(dept_w) << p.department
                << std::setw(rank_w) << p.rank << std::setw(pos_w) << p.position << std::setw(task_w) << p.task;
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
