#include "product_management.h"

void ProductManagement::addProduct(const std::string &name, size_t id, float entryPrice, unsigned int quantity,
                    unsigned int expirationDate = getToday(), float sellingPrice = 0, const std::string &currency = "USD",
                    const std::vector<std::string> &category = {}, const std::string &supplier = "") {
	nlist_insert(nl0, Product{
		.name = name,
        .id = id,
        .entryPrice = entryPrice,
        .sellingPrice = sellingPrice != 0.0f ? sellingPrice : entryPrice,
        .currency = currency,
        .category = category,
        .quantity = quantity,
        .supplier = supplier,
	}, expirationDate);
}

bool ProductManagement::removeProduct(size_t id) {
	return nlist_remove_range(nl0, id);
}

bool ProductManagement::updateProduct(size_t id, unsigned int expirationDate, const std::string &name = "",
					float entryPrice = -1, int quantity = -1,
                    float sellingPrice = -1, const std::string &currency = "",
                    const std::vector<std::string> &category = {}, const std::string &supplier = "") {
	Product *prod = nlist_get(nl0, id, expirationDate, expirationDate+1);
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

bool ProductManagement::warnProduct(const Product& prod) {
	return prod.quantity > 0;
}

bool ProductManagement::warnProduct(size_t id) {
	Product *prod = nlist_get(nl0, id);
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

	category_print(cate_root);
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