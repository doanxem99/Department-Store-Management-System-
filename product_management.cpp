void ProductManagement::listProduct(Node_List *node_list) {
	Category *cate_root = new Category;

	Node_List *iter = node_list;
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

bool ProductManagement::listExpiredProduct(Node_List *node_list) {
	unsigned int today = getToday();
	Node_List *iter = node_list;
	while(iter != nullptr && iter->expirationDate < today) {
		print_node_tree(iter->root, 0);
		iter = iter->next;
	}

	return true;
}

bool ProductManagement::listOutOfStockProduct(Node_List *node_list) {
	Node_List *iter = node_list;
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
					printf("%s(%ld)\n", queue[i]->prod.name.c_str(), queue[i]->prod.id);
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
