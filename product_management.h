// Copyright [2024] <Your name>
#ifndef PRODUCT_MANAGEMENT_H_
#define PRODUCT_MANAGEMENT_H_

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Product {
    std::string name;
    size_t id;
    float entryPrice;
    float sellingPrice;
    std::string currency;  // default is USD
    std::vector<std::string> category;    // Category can go from
                                // food -> fruit -> apple
                                //, non-food -> electronics -> phone
    unsigned int quantity;
    std::string supplier;
};

#define LEFT		0
#define RIGHT		1
#define RANGE_END	1000000000
#define RANGE_START 0


struct Node {
	Product prod;
	size_t height;
	Node *child[2];
};

struct Node_List {
	unsigned int expirationDate;
	unsigned int count;
	size_t minId;
	size_t maxId;
	Node *root;
	Node_List *next;
};

struct Category {
	std::string name;
	std::vector<std::string> list;
	std::vector<Category*> child;
};

static Node_List *nl0 = nullptr;
void nlist_delete_range(Node_List*&, size_t, size_t);

class ProductManagement {
 public:
 	void importProduct(const std::string &infile);
 	void exportProduct(const std::string &outfile);
 	ProductManagement() {
		importProduct("product_list.txt");
 	}
    ~ProductManagement() {
		exportProduct("product_list.txt");
    	nlist_delete_range(nl0, RANGE_END, RANGE_START);
    }

 	void addProduct (unsigned int, Product);
    void addProduct(const std::string&, size_t, float, unsigned int,		// Information is about the product name, ID,
                    unsigned int expirationDate, float, const std::string&,	//  price, quantity, expiration date, etc.
                    const std::vector<std::string>&, const std::string&);

    bool removeProduct(size_t);
    bool updateProduct(size_t, unsigned int, const std::string&,
					float, int, float, const std::string&,
                    const std::vector<std::string>&, const std::string&);
    void listProduct();			     			// List all products in the system
                            					// in a tree-like structure

    bool searchProduct(size_t);   				// Search for a product by name, ID, etc.
    bool searchProduct(const std::string&);		// by using Hash Table or Binary Search Tree

    void sortProduct(std::vector<Product*>&);     	// Sort products by name, ID, etc.
                            							// by using Quick Sort, Merge Sort, etc.

    bool warnProduct(size_t);		 			// Warn the user when the product is out
    bool warnProduct(const Product&);          	// of stock or the product is about to expire

    bool listExpiredProduct();  				// List all expired products in the system

    bool listOutOfStockProduct();   			// List all out-of-stock
                                    			// products in the system
    // Step-by-step for product management when user choose this option
    void stepbystepForProductManagement();

    // add Supplier and remove Supplier is decided by expense management
};

static unsigned int getToday() {
	time_t t = std::time(0);
	std::tm *today = localtime(&t);
	return (today->tm_year+1900)*10000 + (today->tm_mon+1)*100 + today->tm_mday;
}

#endif  // PRODUCT_MANAGEMENT_H_
