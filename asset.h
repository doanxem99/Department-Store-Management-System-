// Copyright [2024]
#ifndef ASSET_H_
#define ASSET_H_

#include <vector>
#include <string>

#include "./personnel_management.h"
#include "./product_management.h"

class Asset {
 public:
    static Tree<PersonnelManagement::Personnel> personnelTree;
    static std::vector<ProductManagement::Product> productList;

    // If there is any change in an app, add it to historyOfChanges
    static std::vector<std::string> historyOfChanges;

    static int64_t pTotalMoney;


    // Import and export data from and to a file
    // for backup when the system is down
    bool exportProduct();
    bool importProduct();

    bool exportPersonnel();
    bool importPersonnel();

    bool exportHistoryOfChanges();
    bool importHistoryOfChanges();
};



#endif  // ASSET_H_
