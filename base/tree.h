#ifndef __TREE_H__
#define __TREE_H__

#include<initializer_list>

using namespace std;

template<class K, class V>
class Tree final {
	using Entry = pair<K, V>;
	enum class Color  {Black, Red};
	friend inline ostream& operator<<(ostream& os, Color c) {
		switch (c) {
		case Color::Black:
			return os << "Black";
		case Color::Red:
			return os << "Red";
		default: return os << static_cast<int>(c);
		}
	}
	template<class Key, class Value>
	struct Node {
		std::pair<Key, Value> data;
		Color color;
		Node* left;
		Node* right;
		Node* parent;
		Node(pair<Key, Value>& d) : Node(d, Color::Black, nullptr, nullptr, nullptr) {}
		Node(pair<Key, Value>& d, Color c, Node* l, Node* r, Node* p) : data(d), color(c), left(l), right(r), parent(p) {}
	};
private:
	Node<K,V>* root;
	size_t size;
public: 
	Tree();
	Tree(const Tree& tree);
	Tree(Tree&& tree) noexcept;
	~Tree();
	void insert(Entry elem);
	void insert(initializer_list<Entry> list);
	void remove(const K& key);
	void remove(initializer_list<K> list);
	bool include(const K& key) const;
	void emplace(const K& key, const V& value);
	void clear();

	bool operator==(const Tree& tree) const;
	bool operator!=(const Tree& tree) const;
	Tree<K, V>& operator=(const Tree& tree);
	Tree<K, V>& operator=(Tree&& tree) noexcept;
	const V& operator[](const K& key) const;
	V& operator[](const K& key);

	size_t get_size() const noexcept;

private:
	void destroy(Node<K, V>* &node);
	void rotate_left(Node<K, V>* &root, Node<K, V>* &x);
	void rotate_right(Node<K, V>* &root, Node<K, V>* &y);
	void insert(Node<K, V>*& root, Node<K, V>*& node);
	void insert_fix(Node<K, V>*& root, Node<K, V>*& node);
	void remove(Node<K, V>*& root, Node<K, V>*& node);
	void remove_fix(Node<K, V>*& root, Node<K, V>*& node, Node<K, V>*& par);
	Node<K, V>* min() const;
	Node<K, V>* max() const;
	Node<K, V>* search(Node<K, V>* node, const K& key) const;

	void print(Node<K, V>* node, ostream& os) ;
public:
	template<class U>
	class Iterator {	

		Node<U, V>* it;
	public:
		Iterator(Node<U, V>* node) :it(node) {}

		U& operator*() { return it->data.first; }
		U* operator->() { return &it->data.first; }
		U& operator*() const { return it->data.first; }
		U* operator->() const { return &it->data.first; }

		bool operator!=(const Iterator& iter) const { return it != iter.it; }
		bool operator==(const Iterator& iter) const { return it == iter.it; }

		Iterator& operator++() {
			if(it->right) {
				it = it->right;
				while (it->left) it = it->left;
			}
			else {
				Node<U, V>* par = it->parent;
				while (par && it == par->right) {
					it = par;
					par = par->parent;
				}
				it = par;
			}
			return *this;
		}

		Iterator<U>& operator--() {
			if (it->left) {
				it = it->left;
				while (it->right) it = it->right;
			}
			else {

				Node<U, V>* par = it->parent;
				while (it == par->left) {

					it = par;
					par = par->parent;
				}
				it = par;
			}
			return *this;
		}

		Iterator<U> operator++(int) { Iterator<U> res(*this); ++(*this); return res; }
		Iterator<U> operator--(int) { Iterator<U> res(*this); --(*this); return res; }
	};

	Iterator<K> begin() { return Iterator<K>(min()); }
	Iterator<K> end() { return Iterator<K>(nullptr); }
	Iterator<K> begin() const { return Iterator<K>(min()); }
	Iterator<K> end() const { return Iterator<K>(const_cast<Node<K, V>*>(root->parent)); }
	const Iterator<const K> cbegin() const{ return Iterator<K>(min());; }
	const Iterator<const K> cend() const { return Iterator<K>(root->parent); }

	friend ostream& operator<<(ostream& os,Tree<K, V>& tree) {
		tree.print(tree.root, os);
		return os;
	}

	friend void swap(Tree<K, V>& lhs, Tree<K, V>& rhs) {
		swap(lhs.root, rhs.root);
		swap(lhs.size, rhs.size);
	}
};

template<class K, class V>
Tree<K, V>::Tree() : root(nullptr), size(0) {}

template<class K, class V>
inline Tree<K, V>::Tree(const Tree& tree) : Tree() {
	for (const auto& a : tree) this->insert({ a, tree[a] });
}

template<class K, class V>
inline Tree<K, V>::Tree(Tree&& tree) noexcept : Tree() {
	swap(*this, tree);
	tree.~Tree();
}
template<class K, class V>
Tree<K, V>::~Tree() { destroy(root); size = 0; }

template<class K, class V>
inline void Tree<K, V>::insert(Entry elem) {
	if (include(elem.first)) throw logic_error("key already used");
	Node<K, V>* z = new Node<K, V>(elem, Color::Red, nullptr, nullptr, nullptr);
	insert(root, z);
	size++;
}

template<class K, class V>
void Tree<K, V>::insert(initializer_list<Entry> list) { for (auto& a : list) insert(a); }

template<class K, class V>
void Tree<K, V>::remove(const K& key) {
	Node<K, V>* deletenode = search(root, key);
	if (!deletenode) return;
	if (deletenode) remove(root, deletenode);
	size--;
}

template<class K, class V>
void Tree<K, V>::remove(initializer_list<K> list) { for (auto& a : list) remove(a); }

template<class K, class V>
inline bool Tree<K, V>::include(const K& key) const  {
	if (!root) return false;
	if (root->data.first == key) return true;
	auto res = search(root, key);
	return res;
}

template<class K, class V>
void Tree<K, V>::emplace(const K& key, const V& value) { insert({ key, value }); }

template<class K, class V>
void Tree<K, V>::clear() { destroy(root); size = 0; }

template<class K, class V>
inline bool Tree<K, V>::operator==(const Tree& tree) const {
	if (size != tree.size) return false;
	for (auto i = begin(), j = tree.begin(); i != end(); i++, j++)
		if (*i != *j) return false;
	return true;
}

template<class K, class V>
inline bool Tree<K, V>::operator!=(const Tree& tree) const { return !(*this == tree); }

template<class K, class V>
Tree<K, V>& Tree<K, V>::operator=(const Tree& tree) {
	if (*this != tree) {
		Tree<K, V> tmp(tree);
		swap(*this, tmp);
	}
	return *this;
}

template<class K, class V>
Tree<K, V>& Tree<K, V>::operator=(Tree&& tree) noexcept {
	if (*this != tree) {
		swap(*this, tree);
		tree.~Tree();
	}
	return *this;
}

template<class K, class V>
const V& Tree<K, V>::operator[](const K& key) const {
	if (root->data.first == key) return root->data.second;
	Node<K, V>* res = search(root, key);
	if (res) return res->data.second;
	else throw logic_error("no element with this key");
}

template<class K, class V>
V& Tree<K, V>::operator[](const K& key) {
	if (root->data.first == key) return root->data.second;
	Node<K, V>* res = search(root, key);
	if (res) return res->data.second;
	else throw logic_error("no element with this key");
}

template<class K, class V>
size_t Tree<K, V>::get_size() const noexcept { return size; }

template<class K, class V>
inline void Tree<K, V>::destroy(Node<K, V>*& node) {
	if (!node) return;
	destroy(node->left);
	destroy(node->right);
	delete node;
	node = nullptr;
}

template<class K, class V>
inline void Tree<K, V>::rotate_left(Node<K, V>* &root, Node<K, V>* &x) {
	Node<K, V>* y = x->right;
	x->right = y->left;
	if (y->left) y->left->parent = x;
	y->parent = x->parent;
	if (!x->parent) root = y;
	else {
		if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

template<class K, class V>
inline void Tree<K, V>::rotate_right(Node<K, V>*& root, Node<K, V>*& y) {
	Node<K, V>* x = y->left;
	y->left = x->right;
	if (x->right) x->right->parent = y;
	x->parent = y->parent;
	if (!y->parent) root = x;
	else {
		if (y == y->parent->right)
			y->parent->right = x;
		else
			y->parent->left = x;
	}
	x->right = y;
	y->parent = x;
}

template<class K, class V>
inline void Tree<K, V>::insert(Node<K, V>*& root, Node<K, V>*& node) {
	Node<K, V>* x = root;
	Node<K, V>* y = nullptr;
	while (x) {
		y = x;
		x = (node->data.first > x->data.first) ? x->right : x->left;
	}
	node->parent = y;
	if (y) {
		if (node->data.first > y->data.first)
			y->right = node;
		else
			y->left = node;
	}
	else root = node;
	node->color = Color::Red;
	insert_fix(root, node);
}

template<class K, class V>
inline void Tree<K, V>::insert_fix(Node<K, V>*& root, Node<K, V>*& node) {
	Node<K, V>* parent;
	parent = node->parent;
	while (node != this->root && parent->color == Color::Red) {
		Node<K, V>* gparent = parent->parent;
		if (gparent->left == parent) {
			Node<K, V>* uncle = gparent->right;
			if (uncle && uncle->color == Color::Red) {
				parent->color = Color::Black;
				uncle->color = Color::Black;
				gparent->color = Color::Red;
				node = gparent;
				parent = node->parent;
			}
			else {
				if (parent->right == node) {
					rotate_left(root, parent);
					swap(node, parent);
				}
				rotate_right(root, gparent);
				gparent->color = Color::Red;
				parent->color = Color::Black;
				break;
			}
		}
		else {
			Node<K, V>* uncle = gparent->left;
			if (uncle && uncle->color == Color::Red) {
				gparent->color = Color::Red;
				parent->color = Color::Black;
				uncle->color = Color::Black;
				node = gparent;
				parent = node->parent;
			}
			else {
				if (parent->left == node) {
					rotate_right(root, parent);
					swap(parent, node);
				}
				rotate_left(root, gparent);
				parent->color = Color::Black;
				gparent->color = Color::Red;
				break;
			}
		}
	}
	root->color = Color::Black;
}

template<class K, class V>
void Tree<K, V>::remove(Node<K, V>*& root, Node<K, V>*& node) {
	if (this->root == node) {
		this->root = nullptr;
		size--;
		return;
	}
	Node<K, V>* child, *parent;
	Color color;
	if (node->left && node->right)
	{
		Node<K, V>* replace = node;
		replace = node->right;
		while (replace->left) {
			replace = replace->left;
		}
		if (node->parent) {
			if (node->parent->left == node)
				node->parent->left = replace;
			else
				node->parent->right = replace;
		}
		else root = replace;
		child = replace->right;
		parent = replace->parent;
		color = replace->color;
		if (parent == node) parent = replace;
		else {
			if (child) child->parent = parent;
			parent->left = child;
			replace->right = node->right;
			node->right->parent = replace;
		}
		replace->parent = node->parent;
		replace->color = node->color;
		replace->left = node->left;
		node->left->parent = replace;
		if (color == Color::Black)
			remove_fix(root, child, parent);

		delete node;
		return;
	}
	if (node->left) child = node->left;
	else child = node->right;
	parent = node->parent;
	color = node->color;
	if (child) child->parent = parent;
	if (parent) {
		if (node == parent->left)
			parent->left = child;
		else
			parent->right = child;
	}
	else this->root = child;
	if (color == Color::Black) remove_fix(root, child, parent);
	delete node;
}

template<class K, class V>
inline void Tree<K, V>::remove_fix(Node<K, V>*& root, Node<K, V>*& node, Node<K, V>*& parent) {
	Node<K, V>* othernode;
	while ((!node) || node->color == Color::Black && node != root) {
		if (parent->left == node) {
			othernode = parent->right;
			if (othernode->color == Color::Red)	{
				othernode->color = Color::Black;
				parent->color = Color::Red;
				rotate_left(root, parent);
				othernode = parent->right;
			}
			else {
				if (!(othernode->right) || othernode->right->color == Color::Black) {
					othernode->left->color = Color::Black;
					othernode->color = Color::Red;
					rotate_right(root, othernode);
					othernode = parent->right;
				}
				othernode->color = parent->color;
				parent->color = Color::Black;
				othernode->right->color = Color::Black;
				rotate_right(root, parent);
				node = root;
				break;
			}
		}
		else {
			othernode = parent->left;
			if (othernode->color == Color::Red) {
				othernode->color = Color::Black;
				parent->color = Color::Red;
				rotate_right(root, parent);
				othernode = parent->left;
			}
			if ((!othernode->left || othernode->left->color == Color::Black) && (!othernode->right || othernode->right->color == Color::Black)) {
				othernode->color = Color::Red;
				node = parent;
				parent = node->parent;
			}
			else
			{
				if (!(othernode->left) || othernode->left->color == Color::Black)
				{
					othernode->right->color = Color::Black;
					othernode->color = Color::Red;
					rotate_left(root, othernode);
					othernode = parent->left;
				}
				othernode->color = parent->color;
				parent->color = Color::Black;
				othernode->left->color = Color::Black;
				rotate_right(root, parent);
				node = root;
				break;
			}
		}
	}
	if (node) node->color = Color::Black;
}

template<class K, class V>
typename Tree<K, V>::template Node<K, V>* Tree<K, V>::search(Node<K, V>* node,const K& key) const {
	if (!node || node->data.first == key) return node;
	else {
		if (key > node->data.first)
			return search(node->right, key);
		else
			return search(node->left, key);
	}
}

template<class K, class V>
void Tree<K, V>::print(Node<K, V>* node, ostream& os) {
	if (!node)
		return;
	if (!(node->parent))
		os << '(' << node->data.first << ';' << node->data.second << ')' << "(" << node->color << ") is root" << endl;
	else if (node->parent->left == node) {
		os << '(' << node->data.first << ';' << node->data.second << ')' << "(" << node->color 
			<< ") is (" << node->parent->data.first << ';' << node->parent->data.second 
			<< ")'s " << "left child" << endl;
	}
	else {
		os << '(' << node->data.first << ';' << node->data.second << ')' << "(" << node->color 
			<< ") is (" << node->parent->data.first << ';' << node->parent->data.second 
			<< ")'s " << "right child" << endl;
	}
	print(node->left, os);
	print(node->right, os);
}

template<class K, class V>
typename Tree<K, V>::template Node<K, V>* Tree<K, V>::min() const {
	Node<K, V>* res = root;
	while (res && res->left) res = res->left;
	return res;
}

template<class K, class V>
typename Tree<K, V>::template Node<K, V>* Tree<K, V>::max() const {
	Node<K, V>* res = root;
	while (res->right) res = res->right;
	return res;
}













#endif


