// Copyright [2024] <Your name>
#ifndef PRODUCT_MANAGEMENT_H_
#define PRODUCT_MANAGEMENT_H_

#include <string>
#include <vector>

#include "./asset.h"

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
    // Step-by-step for product management when user choose this option
    void stepbystepForProductManagement();

    // add Supplier and remove Supplier is decided by expense management
};

#define LEFT	0
#define RIGHT	1
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
		if (iter->prod.id == id) {
			return &iter->prod;
		}

		int dir = id > iter->prod.id;
		iter = iter->child[dir];
	}

	return nullptr;
}

void print_node_nlr(Node *node) {
	if (node == nullptr) {
		return;
	}

	printf("%s - %ld -> %d (%ld)\n", node->prod.name.c_str(), node->prod.id, node->prod.quantity, node->height);
	print_node_nlr(node->child[LEFT]);
	print_node_nlr(node->child[RIGHT]);
}

void print_node_tree(const Node *node, unsigned int space) {
	if (node == nullptr) {
		return;
	}

	printf("%s(%ld)\n", node->prod.name.c_str(), node->prod.id);
	if (node->child[RIGHT] != NULL) {
		printf("%*s|-> ", space, "");
		print_node_tree(node->child[RIGHT], space + 4);
	}

	if (node->child[LEFT] != NULL) {
		printf("%*s\\__ ", space, "");
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

void nlist_delete_range(Node_List *&node_list, size_t end = 1000000000, size_t start = 0) {
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

Product *nlist_get(Node_List *node_list, size_t id) {
	Node_List *iter = node_list;
	while (iter != nullptr) {
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

void nlist_remove_range(Node_List *&node_list, size_t id, size_t start = 0, size_t end = 1000000000) {
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
			}
			iter = iter->next;
		}
	}
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
	iter->list.push_back(name);
}

void category_print(const Category *cate) {
	if (cate == nullptr) {
		return;
	}

	printf("%s:", cate->name.c_str());
	for (const std::string &prod_name: cate->list) {
		printf("%s,", prod_name.c_str());
	}
	for (const Category *child: cate->child) {
		category_print(child);
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

#endif  // PRODUCT_MANAGEMENT_H_
