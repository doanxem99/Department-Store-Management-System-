// Copyright [2024] <Your name>
#ifndef PERSONNEL_MANAGEMENT_H_
#define PERSONNEL_MANAGEMENT_H_

#include <string>

#include "./asset.h"
class PersonnelManagement {
 private:
    PersonnelManagement();
    ~PersonnelManagement();
    void addPersonnel();
    bool removePersonnel();
    bool updatePersonnel();
    void listPersonnel();       // List all personnel in the
                                //  system in a tree-like structure
    bool searchPersonnel();     //  Search for a personnel by name, ID, etc.
                                //  by using Hash Table or Binary Search Tree
    void sortPersonnel();  // Sort personnel by name, ID, etc.
                           // by using Quick Sort, Merge Sort, etc.

    void scheduleWork();  // Schedule work for personnel
    void assignTask();  // Assign task to personnel
    void updateTask();  // Update task for personnel
    void listTask();  // List all tasks in the system in a table-like structure

    void reportByUser();    // Report by user when they want to
                            // change salary, work hour, etc.
 public:
    struct Personnel {
        std::string name;
        std::string ID;
        std::string address;
        std::string phone;
        std::string email;
        std::string position;
        std::string department;
        std::string task;
        std::string work;
        int salary;
        std::string currency;  // default is USD
        int workHour;
        int extraHour;
        int totalHour;
    };


    // Step-by-step for personnel management when user choose this option
    void stepbystepForPersonnelManagement();

    // calculate salary is decided by expense management
};


#endif  // PERSONNEL_MANAGEMENT_H_
