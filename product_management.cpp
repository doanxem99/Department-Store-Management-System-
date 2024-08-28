#include "product_management.h"

Node *node_new(const Product &prod) {
	Node *node = new Node;
	node->prod = prod;
	node->height = 1;
	node->child[LEFT] = node->child[RIGHT] = nullptr;
	return node;
}

void node_delete(Node *&node) {
	if (node == nullptr) {
		return;
	}

	node_delete(node->child[LEFT]);
	node_delete(node->child[RIGHT]);
	delete node;
}

size_t node_height(const Node* node) {
	return node != nullptr ? node->height : 0;
}

Node *node_lrotate(Node* node) {
	Node *r = node->child[RIGHT];
	node->child[RIGHT] = r->child[LEFT];
	r->child[LEFT] = node;

	node->height = std::max(node_height(node->child[LEFT]), node_height(node->child[RIGHT])) + 1;
	r->height = std::max(node_height(r->child[LEFT]), node_height(r->child[RIGHT])) + 1;

	return r;
}

Node *node_rrotate(Node* node) {
	Node *l = node->child[LEFT];
	node->child[LEFT] = l->child[RIGHT];
	l->child[RIGHT] = node;

	node->height = std::max(node_height(node->child[LEFT]), node_height(node->child[RIGHT])) + 1;
	l->height = std::max(node_height(l->child[LEFT]), node_height(l->child[RIGHT])) + 1;

	return l;
}

Node *rebalance(Node *node) {
	if (node == nullptr) {
		return nullptr;
	}

	int diff = node_height(node->child[LEFT]) - node_height(node->child[RIGHT]);
	if (diff > 1) {
		Node *l = node->child[LEFT];
		if (node_height(l->child[LEFT]) < node_height(l->child[RIGHT])) {
			node->child[LEFT] = node_lrotate(l);
		}
		return node_rrotate(node);
	}
	if (diff < -1) {
		Node *r = node->child[RIGHT];
		if (node_height(r->child[LEFT]) > node_height(r->child[RIGHT])) {
			node->child[RIGHT] = node_rrotate(r);
		}
		return node_lrotate(node);
	}
	node->height = 1 + std::max(node_height(node->child[LEFT]), node_height(node->child[RIGHT]));
	return node;
}

Node *node_insert(Node *node, const Product &prod, bool *success) {
	if (node == nullptr) {
		return node_new(prod);
	}
	if (prod.id == node->prod.id) {
		if (success != nullptr) {
			*success = false;
		}
		node->prod.quantity += prod.quantity;
		return node;
	}

	int dir = prod.id > node->prod.id;
	node->child[dir] = node_insert(node->child[dir], prod, success);

	return rebalance(node);
}

Node *node_remove(Node *node, size_t id, size_t *deleted_id) {
	if (node == nullptr) {
		return nullptr;
	}

	if (node->prod.id != id) {
		int dir = id > node->prod.id;
		node->child[dir] = node_remove(node->child[dir], id, deleted_id);
	}
	else {
		if (deleted_id != nullptr) {
			*deleted_id = node->prod.id;
		}
		if (node->child[LEFT] == nullptr && node->child[RIGHT] == nullptr) {
			delete node;
			node = nullptr;
		}
		else if (node->child[RIGHT] == nullptr) {
			Node *tmp = node;
			node = node->child[LEFT];
			delete tmp;
		}
		else if (node->child[LEFT] == nullptr) {
			Node *tmp = node;
			node = node->child[RIGHT];
			delete tmp;
		}
		else {
			Node *leftmost_decendant = node->child[RIGHT];
			while (leftmost_decendant->child[LEFT] != nullptr) {
				leftmost_decendant = leftmost_decendant->child[LEFT];
			}
			node->prod = leftmost_decendant->prod;
			node->child[RIGHT] = node_remove(node->child[RIGHT], leftmost_decendant->prod.id, nullptr);
		}
	}
	if (node == nullptr) {
		return nullptr;
	}

	return rebalance(node);
}

Product *node_get(Node *node, size_t id) {
	Node *iter = node;
	while (iter != nullptr) {
		if (iter->prod.id == id && iter->prod.quantity > 0) {
			return &iter->prod;
		}

		int dir = id > iter->prod.id;
		iter = iter->child[dir];
	}

	return nullptr;
}

Product *node_get_from_name(Node *node, const std::string& name) {
	if (node == nullptr) {
		return nullptr;
	}

	if (node->prod.name == name) {
		return &node->prod;
	}

	Product *prod = node_get_from_name(node->child[LEFT], name);
	if (prod != nullptr) {
		return prod;
	}

	return node_get_from_name(node->child[RIGHT], name);
}

void print_node_nlr(Node *node) {
	if (node == nullptr) {
		return;
	}

	std::cout << node->prod.name << " - " << node->prod.id << " -> " << node->prod.quantity << "(" << node->height << ")\n";
	print_node_nlr(node->child[LEFT]);
	print_node_nlr(node->child[RIGHT]);
}

void print_node_tree(const Node *node, unsigned int space) {
	if (node == nullptr) {
		return;
	}

	std::cout << node->prod.name << "(" << node->prod.id << ")\n";
	if (node->child[RIGHT] != NULL) {
		std::cout << std::string(" ", space) << "|-> ";
		print_node_tree(node->child[RIGHT], space + 4);
	}

	if (node->child[LEFT] != NULL) {
		std::cout << std::string(" ", space) << "\\__ ";
		print_node_tree(node->child[LEFT], space + 4);
	}
}

Node_List *nlist_new(size_t expirationDate, size_t minId = 0, size_t maxId = 0) {
	Node_List *node_list = new Node_List;
	node_list->expirationDate = expirationDate;
	node_list->count = 0;
	node_list->minId = minId;
	node_list->maxId = maxId;
	node_list->root = nullptr;
	node_list->next = nullptr;

	return node_list;
}

void nlist_delete(Node_List *&node_list) {
	node_delete(node_list->root);
	delete node_list;
}

void nlist_delete_range(Node_List *&node_list, size_t end = RANGE_END, size_t start = RANGE_START) {
	Node_List *iter = node_list;
	while (iter != nullptr && iter->expirationDate < start) {
		iter = iter->next;
	}
	if (iter == nullptr) {
		return;
	}
	while (iter != nullptr && iter->expirationDate <= end) {
		Node_List *next = iter->next;
		nlist_delete(iter);
		iter = next;
	}
}

void nlist_link(Node_List *&node_list, Node_List *next) {
	Node_List *iter = node_list;
	while (iter->next != nullptr && iter->expirationDate < next->expirationDate) {
		iter = iter->next;
	}

	if (iter == node_list && node_list->expirationDate > next->expirationDate) {
		next->next = node_list;
		node_list = next;
		return;
	}
	next->next = iter->next;
	iter->next = next;
}

Product *nlist_get(Node_List *node_list, size_t id, size_t end = RANGE_END, size_t start = RANGE_START) {
	Node_List *iter = node_list;
	while (iter != nullptr && iter->expirationDate < start) {
		iter = iter->next;
	}
	while (iter != nullptr && iter->expirationDate <= end) {
		if (id >= iter->minId && id <= iter->maxId) {
			Product *prod = node_get(iter->root, id);
			if (prod != nullptr) {
				return prod;
			}
		}
		iter = iter->next;
	}

	return nullptr;
}

Product *nlist_get_from_name(Node_List *node_list, const std::string &name, size_t end = RANGE_END, size_t start = RANGE_START) {
	Node_List *iter = node_list;
	while (iter != nullptr && iter->expirationDate < start) {
		iter = iter->next;
	}
	while (iter != nullptr && iter->expirationDate <= end) {
		Product *prod = node_get_from_name(iter->root, name);
		if (prod != nullptr) {
			return prod;
		}
		iter = iter->next;
	}

	return nullptr;
}

void nlist_insert(Node_List *&node_list, const Product &prod, unsigned int expirationDate) {
	bool success = true;
	if (node_list == nullptr) {
		node_list = nlist_new(expirationDate, prod.id, prod.id);
		node_list->root = node_insert(node_list->root, prod, &success);
		node_list->count++;
		return;
	}

	Node_List *iter = node_list;
	while (iter->next != nullptr && iter->next->expirationDate < expirationDate) {
		iter = iter->next;
	}

	if (iter->next != nullptr && iter->next->expirationDate == expirationDate) {
		iter = iter->next;
		iter->root = node_insert(iter->root, prod, &success);
		if (success) {
			iter->count++;
			if (iter->minId > prod.id) {
				iter->minId = prod.id;
			}
			if (iter->maxId < prod.id) {
				iter->maxId = prod.id;
			}
		}
		return;
	}

	Node_List *next = nlist_new(expirationDate, prod.id, prod.id);
	next->root = node_insert(next->root, prod, &success);
	next->count++;
	if (iter == node_list && node_list->expirationDate > next->expirationDate) {
		next->next = node_list;
		node_list = next;
		return;
	}
	next->next = iter->next;
	iter->next = next;
}

bool nlist_remove_range(Node_List *&node_list, size_t id, size_t end = RANGE_END, size_t start = RANGE_START) {
	bool found = false;
	Node_List *iter = node_list;
	while (iter != nullptr && iter->expirationDate < start) {
		iter = iter->next;
	}
	if (iter != nullptr) {
		while (iter != nullptr && iter->expirationDate <= end) {
			size_t deleted_id = 0;
			iter->root = node_remove(iter->root, id, &deleted_id);
			if (deleted_id != 0) {
				iter->count--;
				if (deleted_id == iter->minId) {
					iter->minId++;
				}
				if (deleted_id == iter->maxId) {
					iter->maxId--;
				}
				if (iter->minId > iter->maxId || iter->maxId == 0) {
					iter->minId = iter->maxId = 0;
				}
				found = true;
			}
			iter = iter->next;
		}
	}

	return found;
}

void category_insert(Category *c, const std::string &name, const std::vector<std::string> &list) {
	Category *iter = c;
	for (const std::string &category: list) {
		if (iter == nullptr) {
			iter = new Category;
		}

		bool exists = false;
		for (Category *child: iter->child) {
			if (child->name == category) {
				exists = true;
				iter = child;
			}
		}

		if (!exists) {
			Category *cate_new = new Category;
			cate_new->name = category;
			iter->child.push_back(cate_new);
			iter = cate_new;
		}
	}

	for (const std::string &cate_name: iter->list) {
		if (cate_name == name) {
			return;
		}
	}
	iter->list.push_back(name);
}

void category_print(const Category *cate, int space) {
	if (cate == nullptr) {
		return;
	}

	if (cate->name.size() > 0) {
		printf("%*s%s: ", space, "", cate->name.c_str());
		space += 4;
	}
	if (cate->list.size() > 0) {
		for (const std::string &prod_name: cate->list) {
			printf("%s,", prod_name.c_str());
		}
	}
	printf("\n");
	for (const Category *child: cate->child) {
		category_print(child, space);
	}
}

void category_delete(Category *cate) {
	if (cate == nullptr) {
		return;
	}

	for (Category *child: cate->child) {
		category_delete(child);
	}
	delete cate;
}

void ProductManagement::importProduct(const std::string &infile) {
	std::ifstream ifs(infile, std::ios::binary);
	if (!ifs.is_open()) {
		return;
	}

	while (!ifs.eof()) {
		unsigned int expirationDate = 0;
		size_t iter_count = 0;
		ifs >> expirationDate >> iter_count;
		while (iter_count-- > 0) {
			Product prod;
			ifs >> prod.id >> prod.entryPrice >> prod.sellingPrice >> prod.quantity;
			ifs.ignore(2);
			std::getline(ifs, prod.name);
			while (prod.name.back() == '\r') {
				prod.name.pop_back();
			}
			std::getline(ifs, prod.currency);
			while (prod.currency.back() == '\r') {
				prod.currency.pop_back();
			}
			std::getline(ifs, prod.supplier);
			while (prod.supplier.back() == '\r') {
				prod.supplier.pop_back();
			}

			size_t category_size = 0;
			ifs >> category_size;
			ifs.ignore(2);
			while (category_size-- > 0) {
				std::string category;
				std::getline(ifs, category);
				while (category.back() == '\r') {
					category.pop_back();
				}
				prod.category.push_back(category);
			}

			addProduct(expirationDate, prod);
		}
	}
}

void ProductManagement::exportProduct(const std::string &oufile) {
	std::ofstream ofs(oufile);
	if (!ofs.is_open()) {
		std::cout << "ERROR: could not export to " << oufile << "\n";
		return;
	}

	Node_List *iter = nl0;
	std::vector<Node*> queue;
	while (iter != nullptr) {
		while (iter->count == 0) {
			iter = iter->next;
		}
		ofs << iter->expirationDate << " " << iter->count << "\n";
		Node *root = iter->root;
		if (root != nullptr) {
			queue.push_back(root);
		}
		size_t queue_size = queue.size();
		while (queue_size > 0) {
			for (size_t i = 0; i < queue_size; i++) {
				ofs << queue[i]->prod.id << " " << queue[i]->prod.entryPrice << " " << queue[i]->prod.sellingPrice << " "
					<< queue[i]->prod.quantity << "\n"
					<< queue[i]->prod.name << "\n"
					<< queue[i]->prod.currency << "\n"
					<< queue[i]->prod.supplier << "\n";

				ofs << queue[i]->prod.category.size() << "\n";
				for (const std::string &category: queue[i]->prod.category) {
					ofs << category << "\n";
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
}

void ProductManagement::addProduct(const std::string &name, size_t id, float entryPrice, unsigned int quantity,
                    unsigned int expirationDate, float sellingPrice = 0, const std::string &currency = "USD",
                    const std::vector<std::string> &category = {}, const std::string &supplier = "") {
	nlist_insert(nl0, Product{ name, id,
		        entryPrice, sellingPrice != 0.0f ? sellingPrice : entryPrice,
		        currency, category.size() > 0 ? category : std::vector<std::string>{"Uncategorized"}, quantity, supplier,
				}, expirationDate);
}

void ProductManagement::addProduct(unsigned int expirationDate, Product prod) {
	if (prod.category.size() == 0) {
		prod.category.push_back("Uncategorized");
	}
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
	if (l >= list.size() || r >= list.size()) {
		return;
	}

    size_t n1 = m - l + 1;
    size_t n2 = r - m;

    for (size_t i = 0; i < n1; i++) {
        L[i] = list[l + i];
    }
    for (size_t i = 0; i < n2; i++) {
        R[i] = list[m + 1 + i];
    }

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
		for (size_t i = 0; idx.size() >= step*(i+2) && i < idx.size() - step*(i+2); i += step*2) {
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
    int choice = 0;
	do {
		std::cout << "\t\t\tPRODUCT MANAGEMENT\n";
	    std::cout << "1. Add a product\n";
	    std::cout << "2. Remove a product\n";
	    std::cout << "3. Update product\n";
	    std::cout << "4. Print a list of products as tree-like\n";
	    std::cout << "5. Print a list of products in increasingly id order\n";
	    std::cout << "6. Print a list of expired products\n";
	    std::cout << "7. Print a list of out of stock products\n";
	    std::cout << "8. Search for a product\n";
	    std::cout << "9. Exit\n\n";

	    do {
	    	std::cout << "Your choice: ";
	    	std::cin >> choice;
	    } while (choice < 1 || choice > 9);

	    if (choice == 1) {
	    	Product prod;
	    	std::cout << "Name: ";
	    	std::cin.ignore();
	    	std::getline(std::cin, prod.name);
	    	std::cout << "Id: ";
	    	std::cin >> prod.id;
	    	std::cout << "Entry price: ";
	    	std::cin >> prod.entryPrice;
	    	std::cout << "Quantity: ";
	    	std::cin >> prod.quantity;
	    	std::cout << "Expiration date (format yyyymmdd): ";
	    	unsigned int expirationDate = 0;
	    	std::cin >> expirationDate;
	    	std::string line;
	    	std::cout << "Selling price (press ENTER to set default is " << prod.entryPrice*1.5f << ")\n";
	    	std::cin.ignore();
    		std::getline(std::cin, line);
    		if (line.size() == 0) {
    			prod.sellingPrice = prod.entryPrice*1.5f;
    		}
    		else {
    			prod.sellingPrice = atoi(line.c_str());
    		}
	    	std::cout << "Currency (press ENTER to set default is USD)\n";
    		std::getline(std::cin, line);
    		if (line.size() == 0) {
    			prod.currency = "USD";
    		}
    		else {
    			prod.currency = line;
    		}
	    	std::cout << "Category (leave empty and press ENTER to stop)\n";
	    	do {
	    		std::getline(std::cin, line);
	    		if (line.size() >= 3) {
	    			prod.category.push_back(line);
	    		}
	    	} while (line.size() > 0);

	    	std::cout << "Supplier (press ENTER to skip)\n";
	    	std::getline(std::cin, line);
    		prod.supplier = line;
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
	    	std::cin.ignore();
	    	std::string line;
	    	std::cout << "Range start (format yyyymmdd or leave empty to search from beginning): ";
	    	unsigned int range_start = 0;
	    	std::getline(std::cin, line);
	    	if (line.size() == 0) {
	    		range_start = RANGE_START;
	    	}
	    	else {
	    		range_start = atoi(line.c_str());
	    	}
	    	std::cout << "Range end (format yyyymmdd or leave empty to search to the last): ";
	    	unsigned int range_end = 0;
	    	std::getline(std::cin, line);
	    	if (line.size() == 0) {
	    		range_end = RANGE_END;
	    	}
	    	else {
	    		range_end = atoi(line.c_str());
	    	}

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
	    		std::cin.ignore();
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
	    	std::vector<Product*> list;
	    	sortProduct(list);
	    	for (const Product* prod: list) {
	    		std::cout << prod->name << "(" << prod->id << "), quantity: " << prod->quantity << ", category: [";
	    		for (size_t i = 0; i < prod->category.size() - 1; i++) {
	    			std::cout << prod->category[i] << ", ";
	    		}
	    		std::cout << prod->category.back() << "]\n";
	    	}
	    }
	    else if (choice == 6) {
	    	listExpiredProduct();
	    }
	    else if (choice == 7) {
	    	listOutOfStockProduct();
	    }
	    else if (choice == 8) {
	    	std::string info;
	    	std::cin.ignore();
	    	std::getline(std::cin, info);

	    	bool is_number = true;
	    	for (char c: info) {
	    		if (c < '0' || c > '9') {
	    			is_number = false;
	    			break;
	    		}
	    	}

	    	std::string line;
	    	std::cout << "Range start (format yyyymmdd or leave empty to search from beginning): ";
	    	unsigned int range_start = 0;
	    	std::getline(std::cin, line);
	    	if (line.size() == 0) {
	    		range_start = RANGE_START;
	    	}
	    	else {
	    		range_start = atoi(line.c_str());
	    	}
	    	std::cout << "Range end (format yyyymmdd or leave empty to search to the last): ";
	    	unsigned int range_end = 0;
	    	std::getline(std::cin, line);
	    	if (line.size() == 0) {
	    		range_end = RANGE_END;
	    	}
	    	else {
	    		range_end = atoi(line.c_str());
	    	}

	    	Product *prod = nullptr;
	    	if (is_number) {
	    		size_t num = std::atoll(info.c_str());
	    		prod = nlist_get(nl0, num, range_end, range_start);
	    	}
	    	else {
	    		prod = nlist_get_from_name(nl0, info, range_end, range_start);
	    	}

	    	if (prod != nullptr) {
	    		std::cout << "Name: " << prod->name << "\n";
	    		std::cout << "Id: " << prod->id << "\n";
	    		std::cout << "Quantity: " << prod->quantity << "\n";
	    		std::cout << "Entry price: " << prod->entryPrice << " " << prod->currency << "\n";
	    		std::cout << "Selling price: " << prod->sellingPrice << " " << prod->currency << "\n";
	    		std::cout << "Category: ";
	    		for (size_t i = 0; i < prod->category.size() - 1; i++) {
	    			std::cout << prod->category[i] << ", ";
	    		}
	    		std::cout << prod->category.back() << "\n";
	    		std::cout << "Supplier: " << prod->supplier << "\n";
	    	}
	    	else {
	    		std::cout << "Not found\n";
	    	}
	    }
	} while (choice != 9);
}