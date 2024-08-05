// Copyright [2024] <Your name>
#ifndef PRODUCT_MANAGEMENT_H_
#define PRODUCT_MANAGEMENT_H_

#include <string>
#include <vector>

#include "./asset.h"

class ProductManagement {
 private:
    ProductManagement();
    ~ProductManagement();
    void addProduct();  // Information is about the product name, ID,
                        //  price, quantity, expiration date, etc.
    bool removeProduct();
    bool updateProduct();
    void listProduct();     // List all products in the system
                            // in a tree-like structure

    bool searchProduct();   // Search for a product by name, ID, etc.
                            // by using Hash Table or Binary Search Tree

    void sortProduct();     // Sort products by name, ID, etc.
                            // by using Quick Sort, Merge Sort, etc.

    bool warnProduct();     // Warn the user when the product is out
                            // of stock or the product is about to expire

    bool listExpiredProduct();  // List all expired products in the system

    bool listOutOfStockProduct();   // List all out-of-stock
                                    // products in the system

    void ongoingOrder();    // List all ongoing orders in the system

    void completedOrder();  // List all completed orders in the system

    void reportByUser();    // Report by user when they want to
                            // annouce something about the product

 public:
    struct Product {
        std::string name;
        std::string ID;
        int entryPrice;
        int sellingPrice;
        std::string currency;  // default is USD
        std::vector<std::string> category;    // Category can go from
                                    // food -> fruit -> apple
                                    //, non-food -> electronics -> phone
        int quantity;
        std::string expirationDate;
        std::string supplier;
    };
    // Step-by-step for product management when user choose this option
    void stepbystepForProductManagement();

    // add Supplier and remove Supplier is decided by expense management
};


#endif  // PRODUCT_MANAGEMENT_H_
