// CopyRight: Author Nguyen Dinh Manh
#include "./asset.h"

// set up the initial value for the static variable
std::vector<PersonnelManagement::Personnel> Asset::personnelList;
std::vector<ProductManagement::Product> Asset::productList;
std::vector<std::string> Asset::historyOfChanges;
int64_t Asset::pTotalMoney = 0;
