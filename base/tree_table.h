#ifndef __TREE_TABLE_H__
#define __TREE_TABLE_H__

#include"table.h"
#include"tree.h"

template<class K, class V>
class TreeTable : public virtual Table<K, V> {
	template<class Key, class Value>
	using TEntry = pair<Key, Value>;
	using Entry = TEntry<K, V>;

	Tree<K, V> data;
protected:
	void print(ostream& os) override;
	int find(const K& key) const override;
public:
	TreeTable();
	TreeTable(initializer_list<Entry> list);
	TreeTable(const TreeTable& table);
	TreeTable(TreeTable&& table) noexcept;

	void add(const Entry& elem) override;
	void add(initializer_list<Entry> list) override;
	void remove(const K& key) override;
	void remove(initializer_list<K> list) override;
	void emplace(const K& key, const V& value);
	bool include(const K& key) const override;
	size_t get_size() const override;
	bool empty() const noexcept;
	
	const V& operator[](const K& key) const override;
	V& operator[](const K& key) override;
	bool operator ==(const TreeTable& table) const;
	bool operator !=(const TreeTable& table) const;
	TreeTable<K, V>& operator=(const TreeTable& table);
	TreeTable<K, V>& operator=(TreeTable&& table) noexcept;
	friend void swap(TreeTable<K, V>& lhs, TreeTable<K, V>& rhs) { swap(lhs.data, rhs.data); }

	template<class U>
	class Iterator {
		typename Tree<U, V>::template Iterator<U> it;
	public:
		Iterator(typename Tree<U, V>::template Iterator<U> iter) : it(iter) {}

		Iterator& operator++() { ++it; return *this; }
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
		Iterator& operator--() { --it; return *this; }
		Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }

		bool operator==(const Iterator& other) const { return it == other.it; }
		bool operator!=(const Iterator& other) const { return !(*this == other); }

		U& operator*() const { return *it; }
		U* operator->() const { return &it; }
	};
	Iterator<K> begin() { return data.begin(); }
	Iterator<K> end() { return data.end(); }
	Iterator<const K> begin() const { return data.begin(); }
	Iterator<const K> end()  const { return data.end(); }
	Iterator<const K> cbegin() const { return data.cbegin(); }
	Iterator<const K> cend() const { return data.cend(); }

	friend ostream& operator<<(ostream& os,TreeTable<K, V>& table) {
		os << "№\t" << "Ключ:\t" << "Значение:\n";
		size_t i = 1;
		for (auto& a : table.data) {
			os << i << '\t' << a << '\t' << table.data[a] << "\n";
			i++;
		}
		return os;
	}
};

template<class K, class V>
inline TreeTable<K, V>::TreeTable() {};

template<class K, class V>
inline TreeTable<K, V>::TreeTable(initializer_list<Entry> list) { data.insert(list); };

template<class K, class V>
inline TreeTable<K, V>::TreeTable(const TreeTable& table) : data(table.data) {};

template<class K, class V>
inline TreeTable<K, V>::TreeTable(TreeTable&& table) noexcept  : data(std::move(table.data))  {};

template<class K, class V>
void TreeTable<K, V>::add(const Entry& elem) { data.insert(elem); }

template<class K, class V>
void TreeTable<K, V>::add(initializer_list<Entry> list) { data.insert(list); }

template<class K, class V>
void TreeTable<K, V>::remove(const K& key) { data.remove(key); }

template<class K, class V>
void TreeTable<K, V>::remove(initializer_list<K> list) { data.remove(list); }

template<class K, class V>
void TreeTable<K, V>::emplace(const K& key, const V& value) { data.emplace(key, value); }

template<class K, class V>
bool TreeTable<K, V>::include(const K& key) const { return data.include(key); }

template<class K, class V>
size_t TreeTable<K, V>::get_size() const { return data.get_size(); }

template<class K, class V>
bool TreeTable<K, V>::empty() const noexcept { return data.get_size() == 0; }


template<class K, class V>
int TreeTable<K, V>::find(const K& key) const { return -1; }

template<class K, class V>
bool TreeTable<K, V>::operator==(const TreeTable& table) const { return data == table.data; }

template<class K, class V>
bool TreeTable<K, V>::operator!=(const TreeTable& table) const { return data != table.data; }

template<class K, class V>
TreeTable<K, V>& TreeTable<K, V>::operator=(const TreeTable& table) { 
	data = table.data; 
	return *this; 
}

template<class K, class V>
TreeTable<K, V>& TreeTable<K, V>::operator=(TreeTable&& table) noexcept {
	data = move(table.data); 
	return *this; 
}

template<class K, class V>
const V& TreeTable<K, V>::operator[](const K& key) const { return data[key]; }

template<class K, class V>
V& TreeTable<K, V>::operator[](const K& key) { return data[key]; }

template<class K, class V>
void TreeTable<K, V>::print(ostream& os) {
	os << "№\t" << "Ключ:\t" << "Значение:" << endl;
	size_t i = 1;
	for (auto& a : data) {
		os << i << '\t' << a << '\t' << data[a] << endl;
		i++;
	}
}












#endif