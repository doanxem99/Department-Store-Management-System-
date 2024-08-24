// CopyRight: Author Nguyen Dinh Manh
#include "./asset.h"

// set up the initial value for the static variable
Tree<PersonnelManagement::Personnel> Asset::personnelTree;
std::vector<ProductManagement::Product> Asset::productList;
std::vector<std::string> Asset::historyOfChanges;
int64_t Asset::pTotalMoney = 0;
