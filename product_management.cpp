#include "product_management.h"

void ProductManagement::addProduct(const std::string &name, size_t id, float entryPrice, unsigned int quantity,
                    unsigned int expirationDate, float sellingPrice = 0, const std::string &currency = "USD",
                    const std::vector<std::string> &category = {}, const std::string &supplier = "") {
	nlist_insert(nl0, Product{ name, id,
		        entryPrice, sellingPrice != 0.0f ? sellingPrice : entryPrice,
		        currency, category, quantity, supplier,
				}, expirationDate);
}

void ProductManagement::addProduct(unsigned int expirationDate, const Product &prod) {
	nlist_insert(nl0, prod, expirationDate);
}

bool ProductManagement::removeProduct(size_t id) {
	return nlist_remove_range(nl0, id);
}

bool ProductManagement::updateProduct(size_t id, unsigned int expirationDate, const std::string &name = "",
					float entryPrice = -1, int quantity = -1,
                    float sellingPrice = -1, const std::string &currency = "",
                    const std::vector<std::string> &category = {}, const std::string &supplier = "") {
	Product *prod = nlist_get(nl0, id, expirationDate+1, expirationDate);
	if (prod == nullptr) {
		return false;
	}

	prod->name = name != "" ? name : prod->name;
    prod->entryPrice = entryPrice != -1 ? entryPrice : prod->entryPrice;
    prod->sellingPrice = sellingPrice != -1 ? sellingPrice : prod->sellingPrice;
    prod->currency = currency != "" ? currency : prod->currency;
    prod->category = category.size() > 0 ? category : prod->category;
    prod->quantity = quantity < 0 ? quantity : prod->quantity;
    prod->supplier = supplier != "" ? supplier : prod->supplier;
	return true;
}

bool ProductManagement::searchProduct(size_t id) {
	return nlist_get(nl0, id) != nullptr;
}

bool ProductManagement::searchProduct(const std::string &name) {
	return nlist_get_from_name(nl0, name) != nullptr;
}

void product_push_back(Node *node, std::vector<Product*> &list) {
	if (node == nullptr) {
		return;
	}
	product_push_back(node->child[LEFT], list);
	list.push_back(&node->prod);
	product_push_back(node->child[RIGHT], list);
}

void merge(std::vector<Product*> &list, size_t l, size_t m, size_t r, Product **L, Product **R)
{
    size_t n1 = m - l + 1;
    size_t n2 = r - m;

    for (size_t i = 0; i < n1; i++) {
        L[i] = list[l + i];
    }
    for (size_t i = 0; i < n2; i++)
        R[i] = list[m + 1 + i];

    size_t i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i]->id <= R[j]->id) {
            list[k] = L[i++];
        }
        else {
            list[k] = R[j++];
        }
        k++;
    }

    while (i < n1) {
        list[k++] = L[i++];
    }

    while (j < n2) {
        list[k++] = R[j++];
    }
}

void ProductManagement::sortProduct(std::vector<Product*> &list) {
	std::vector<unsigned int> idx;
	Node_List *iter = nl0;
	size_t start_idx = 0;
	while (iter != nullptr) {
		idx.push_back(start_idx);
		start_idx += iter->count;
		product_push_back(iter->root, list);
		iter = iter->next;
	}
	idx.push_back(start_idx);

	Product **L = new Product*[list.size()/2];
    Product **R = new Product*[list.size()/2];
	size_t step = 1;
	while (step <= idx.size()/2) {
		for (size_t i = 0; i < idx.size() - step*(i+2); i += step*2) {
			merge(list, idx[step*i], idx[step*(i+1)], idx[step*(i+2)], L, R);
		}
		step *= 2;
	}

	delete[] L;
	delete[] R;
}

bool ProductManagement::warnProduct(const Product& prod) {
	return prod.quantity > 0;
}

bool ProductManagement::warnProduct(size_t id) {
	Product *prod = nlist_get(nl0, id, 1000000000, getToday());
	if (prod == nullptr) {
		return false;
	}
	return prod->quantity > 0;
}

void ProductManagement::listProduct() {
	Category *cate_root = new Category;

	Node_List *iter = nl0;
	std::vector<Node*> prod_list;
	std::vector<Node*> queue;
	while (iter != nullptr) {
		Node *root = iter->root;
		if (root != nullptr) {
			queue.push_back(root);
		}
		size_t queue_size = queue.size();
		while (queue_size > 0) {
			for (size_t i = 0; i < queue_size; i++) {
				category_insert(cate_root, queue[i]->prod.name, queue[i]->prod.category);
				for (size_t j = 0; j < 2; j++) {
					if (queue[i]->child[j] != nullptr) {
						queue.push_back(queue[i]->child[j]);
					}
				}
			}
			queue.erase(queue.begin(), queue.begin() + queue_size);
			queue_size = queue.size();
		}
		queue.clear();
		iter = iter->next;
	}

	category_print(cate_root, 0);
	category_delete(cate_root);
}

bool ProductManagement::listExpiredProduct() {
	unsigned int today = getToday();
	Node_List *iter = nl0;
	while(iter != nullptr && iter->expirationDate < today) {
		print_node_tree(iter->root, 0);
		iter = iter->next;
	}

	return true;
}

bool ProductManagement::listOutOfStockProduct() {
	Node_List *iter = nl0;
	std::vector<Node*> prod_list;
	std::vector<Node*> queue;
	while (iter != nullptr) {
		Node *root = iter->root;
		if (root != nullptr) {
			queue.push_back(root);
		}
		size_t queue_size = queue.size();
		while (queue_size > 0) {
			for (size_t i = 0; i < queue_size; i++) {
				if (queue[i]->prod.quantity == 0) {
					std::cout << queue[i]->prod.name << "(" << queue[i]->prod.id << ")\n";
				}
				for (size_t j = 0; j < 2; j++) {
					if (queue[i]->child[j] != nullptr) {
						queue.push_back(queue[i]->child[j]);
					}
				}
			}
			queue.erase(queue.begin(), queue.begin() + queue_size);
			queue_size = queue.size();
		}
		queue.clear();
		iter = iter->next;
	}

	return true;
}

void ProductManagement::stepbystepForProductManagement() {
	while (true) {
		std::cout << "\t\t\PRODUCT MANAGEMENT\n";
	    std::cout << "1. Add a product\n";
	    std::cout << "2. Remove a product\n";
	    std::cout << "3. Update product\n";
	    std::cout << "4. Print a list of products\n";
	    std::cout << "5. Print a list of expired products\n";
	    std::cout << "6. Print a list of out of stock products\n";
	    std::cout << "7. Search for a product\n";
	    std::cout << "8. Exit\n\n";

	    int choice = 0;
	    do {
	    	std::cout << "Your choice: ";
	    	std::cin >> choice;
	    } while (choice < 1 || choice > 8);

	    if (choice == 1) {
	    	Product prod;
	    	std::cout << "Name: ";
	    	std::getline(std::cin, prod.name);
	    	std::cout << "Id: ";
	    	std::cin >> prod.id;
	    	std::cout << "Entry price: ";
	    	std::cin >> prod.entryPrice;
	    	std::cout << "Quantity: ";
	    	std::cin >> prod.quantity;
	    	std::cout << "Expiration date: ";
	    	unsigned int expirationDate = 0;
	    	std::cin >> expirationDate;
	    	std::cout << "Selling price (press ENTER to set default is" << prod.entryPrice << " ): ";
	    	std::cin >> prod.sellingPrice;
	    	std::cout << "Currency (press ENTER to set default is USD): ";
	    	std::cin >> prod.currency;
	    	std::cout << "Category (leave empty and press ENTER to stop)\n";
	    	std::string category = " ";
	    	do {
	    		std::getline(std::cin, category);
	    		if (category.size() > 0) {
	    			prod.category.push_back(category);
	    		}
	    	} while (category.size() > 0);

	    	std::cout << "Supplier (press ENTER to skip): ";
	    	std::cin >> prod.supplier;
	    	addProduct(expirationDate, prod);
	    }
	    else if (choice == 2) {
	    	std::cout << "Id: ";
	    	size_t id = 0;
	    	std::cin >> id;
	    	if (removeProduct(id)) {
	    		std::cout << "Remove product success\n";
	    	}
	    	else {
	    		std::cout << "Failed to remove, product not found\n";
	    	}
	    }
	    else if (choice == 3) {
	    	int info_choice = 0;
	    	std::cout << "Id: ";
	    	size_t id = 0;
	    	std::cin >> id;
	    	std::cout << "Range start (format yyyymmdd or leave empty to search from beginning): ";
	    	unsigned int range_start = 0;
	    	std::cin >> range_start;
	    	std::cout << "Range end (format yyyymmdd or leave empty to search to the last): ";
	    	unsigned int range_end = 0;
	    	std::cin >> range_end;

	    	do {
	    		std::cout << "\t\t\t Which information you want to change:\n";
	    		std::cout << "1. Name\n";
	    		std::cout << "2. Selling price\n";
	    		std::cout << "3. Quantity\n";
	    		std::cout << "4. Exit\n";
	    		std::cin >> info_choice;
	    	} while (info_choice != 4);
			std::string name;
			float price = 0;
			unsigned int quantity = 0;
	    	if (info_choice == 1) {
    			std::getline(std::cin, name);
    		}
    		else if (info_choice == 2) {
    			std::cin >> price;
    		}
    		else if (info_choice == 3) {
    			std::cin >> quantity;
    		}
	    	for (size_t i = range_start; i <= range_end; i++) {
	    		Product *prod = nlist_get(nl0, id, i+1, i);
	    		if (info_choice == 1) {
	    			prod->name = name;
	    		}
	    		else if (info_choice == 2) {
	    			prod->sellingPrice = price;
	    		}
	    		else if (info_choice == 3) {
	    			prod->sellingPrice = quantity;
	    		}
	    	}
	    }
	    else if (choice == 4) {
	    	listProduct();
	    }
	    else if (choice == 5) {
	    	listExpiredProduct();
	    }
	    else if (choice == 6) {
	    	listOutOfStockProduct();
	    }
	    else if (choice == 7) {
	    	std::string info;
	    	std::getline(std::cin, info);

	    	bool is_number = true;
	    	for (char c: info) {
	    		if (c < '0' || c > '9') {
	    			is_number = false;
	    			break;
	    		}
	    	}

	    	std::cout << "Range start (format yyyymmdd or leave empty to search from beginning): ";
	    	unsigned int range_start = 0;
	    	std::cin >> range_start;
	    	std::cout << "Range end (format yyyymmdd or leave empty to search to the last): ";
	    	unsigned int range_end = 0;
	    	std::cin >> range_end;

	    	bool exists = false;
	    	if (is_number) {
	    		size_t num = std::atoll(info.c_str());
	    		exists = nlist_get(nl0, num, range_end, range_start);
	    	}
	    	else {
	    		exists = nlist_get_from_name(nl0, info, range_end, range_start);
	    	}

	    	if (exists) {
	    		std::cout << "Found\n";
	    	}
	    	else {
	    		std::cout << "Not found\n";
	    	}
	    }
	    else if (choice == 8) {
	    	break;
	    }
	}
}
