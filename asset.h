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
    static int64_t pTotalMoney;
};

#endif  // ASSET_H_
