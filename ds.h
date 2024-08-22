#include <iostream>
#include <fstream>

template <typename T> class Tree
{
private:
    struct Node
    {
        T key;
        Node* left = nullptr;
        Node* right = nullptr;
    };
    Node* root;
    
    int height(Node* node)
    {
    	if (node == nullptr)
    		return 0;
    	return 1 + std::max(height(node->left), height(node->right));
    }
    
    Node* right_rotate(Node* subroot)
    {
    	Node* child = subroot->left;
    	subroot->left = child->right;
    	child->right = subroot;
    	subroot = child;
    	return subroot;
    }
    
    Node* left_rotate(Node* subroot)
    {
    	Node* child = subroot->right;
    	subroot->right = child->left;
    	child->left = subroot;
    	subroot = child;
    	return subroot;
    }
    
    Node* double_right_rotate(Node* node)//for case LR
    {
    	node->left = left_rotate(node->left);
        return right_rotate(node);
    }
    
    Node* double_left_rotate(Node* node)//for case RL
    {
    	node->right = right_rotate(node->right);
        return left_rotate(node);
    }
    
    int height_diff(Node* node)
    {
        if (node == nullptr)
            return 0;
        return height(node->left) - height(node->right);
    }
    
    Node* rebalance(Node* node)
    {
    	int diff = height_diff(node);
    	if (diff > 1 && height_diff(node->left) > -1)   //case LL
    		return right_rotate(node);
    	if (diff < -1 && height_diff(node->right) < 1)  //case RR
            return left_rotate(node);
        if (diff > 1 && height_diff(node->left) < 0)    //case LR
        	return double_right_rotate(node);
        if (diff < -1 && height_diff(node->right) > 0)  //case RL
        	return double_left_rotate(node);
        return node;
    }
    
    void insert_node(Node* &root, T key)
    {
    	if (root == nullptr)
    	{
    		root = new Node;
    		root->key = key;
    		return;
    	}
    	if (key > root->key)
    		insert_node(root->right, key);
    	else if (key < root->key)
    		insert_node(root->left, key);
    	else
    	    return;
    	root = rebalance(root);
    }
    
    void delete_node(Node* &root, T key)
    {
        if (root == nullptr)
            return;
        if (key > root->key)
            delete_node(root->right, key);
        else if (key < root->key)
            delete_node(root->left, key);
        else
        {
            if (root->right == nullptr)
            {
                Node* temp = root;
                root = root->left;
                delete temp;
            }
            else if (root->left == nullptr)
            {
                Node* temp = root;
                root = root->right;
                delete temp;
            }
            else
            {
                Node* temp = root->right;
    			while (temp->left != nullptr)
    				temp = temp->left;
                root->key = temp->key;
                delete_node(root->right, temp->key);
            }
        }
        if (root == nullptr)
            return;
        root = rebalance(root);
    }

    bool find_node(Node* root, T &key)
    {
        while (root != nullptr)
        {
            if (key > root->key)
                root = root->right;
            else if (key < root->key)
                root = root->left;
            else
            {
                key = root->key;
                return true;
            }
        }
        return false;
    }

    void update_node(Node* &root, T key)
    {
        if (root == nullptr)
    		return;
    	if (key > root->key)
    		update_node(root->right, key);
    	else if (key < root->key)
    		update_node(root->left, key);
    	else
    	{
            root->key = key;
            return;
        }
    }
    
    void delete_tree(Node* root)
    {
    	if (root == nullptr)
    		return;
    	delete_tree(root->left);
    	delete_tree(root->right);
    	delete root;
    }
    
    void print_tree(Node* root)  // inorder traversal
    {
        if (root != nullptr)
        {
            print_tree(root->left);
            std::cout << root->key << '\n';
            print_tree(root->right);
        }
    }

    void export_tree_to_file(Node* root, std::ofstream& fout, void (*print_line_to_file)(ofstream&, T))
    {
        if (root != nullptr)
        {
            export_tree_to_file(root->left, fout, print_line_to_file);
            print_line_to_file(fout, root->key);
            export_tree_to_file(root->right, fout, print_line_to_file);
        }
    }
    
public:
    Tree()
    {
        root = nullptr;
    }

    ~Tree()
    {
        delete_tree(root);
    }
    
    void insert(T key)
    {
        insert_node(root, key);
    }
    
    void erase(T key)
    {
        delete_node(root, key);
    }

    bool find(T &key)
    {
        return find_node(root, key);
    }
    
    void update(T key)
    {
        update_node(root, key);
    }

    bool is_empty()
    {
        return (root == nullptr);
    }
    
    void print()
    {
        print_tree(root);
        std::cout << '\n';
    }

    void export_to_file(std::ofstream& fout, void (*print_line_to_file)(ofstream&, T))
    {
        export_tree_to_file(root, fout, print_line_to_file);
    }
};
