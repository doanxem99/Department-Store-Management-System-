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