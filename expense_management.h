// Copyright [2024] <Your name>
#ifndef EXPENSE_MANAGEMENT_H_
#define EXPENSE_MANAGEMENT_H_

#include <vector>
#include <string>

class ExpenseManagement {
 private:
    void calculateTax();    // Calculate tax for personnel
    void calculateSalary();     // Calculate salary for personnel

    bool addSupplier();  // Add supplier to the system
    bool removeSupplier();  // Remove supplier from the system
    bool updateSupplier();
    void listSupplier();    // List all suppliers
                            // in the system in a table-like structure

    void historyOrder();    // List all orders in the system
    void historyTransaction();  // List all transactions in the system

    void calculateExpense();    // Calculate expense for the store
    void calculateRevenue();    // Calculate revenue for the store

    void reportExpenseDaily();  // Report daily expense
    void reportExpenseMonthly();    // Report monthly expense
    void reportExpenseYearly();     // Report yearly expense

    void exportHistoryOrder();  // Export history order to a file
    void exportHistoryTransaction();     // Export history transaction to a file
    void importHistoryOrder();  // Import history order from a file
    void importHistoryTransaction();   // Import history transaction from a file

 public:
    struct Supplier {
        std::string name;
        std::string ID;
        std::string address;
        std::string phone;
        std::string email;
    };
    struct Order {
        std::string ID;
        std::string productID;
        std::string productName;
        std::string supplierID;
        std::string supplierName;
        int quantity;
        int price;
        std::string currency;  // default is USD
        std::string date;
        std::string status;
    };

    // Step-by-step for expense management when user choose this option
    void stepbystepForExpenseManagement();
};


#endif  // EXPENSE_MANAGEMENT_H_
