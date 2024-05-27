#ifndef __LIST_TABLE_H__
#define __LIST_TABLE_H__

#include "linked_list.h"
#include "table.h"


template<class K, class V>
class ListTable : public virtual Table<K, V>{
	template<class Key, class Value>
	using TEntry = pair<Key, Value>;

	using Entry = TEntry<K, V>;

	LinkedList<Entry> data;
protected:
	void print(ostream& os) override;
public:
	ListTable();
	ListTable(initializer_list<Entry> list);
	ListTable(const ListTable& table);
	ListTable(ListTable&& table);

	
	void add(const Entry& elem) override;
	void add(initializer_list<Entry> list) override;
	void remove(const K& key) override;
	void remove(initializer_list<K> list) override;
	int find(const K& key) const override;
	void emplace(const K& key, const V& value);
	void replace(const K& key, const V& value);
	size_t get_size() const override;
	bool empty() const noexcept;
	bool include(const K& key) const override;

	void operator=(const ListTable& table);
	void operator=(ListTable&& table);
	const V& operator[](const K& key) const override;
	V& operator[](const K& key) override;
	bool operator==(const ListTable<K, V> table) const;
	bool operator!=(const ListTable<K, V> table) const;

	template<class U>
	class Iterator {
		typename LinkedList<TEntry<U,V>>::template Iterator<TEntry<U, V>> it;
	public:
		Iterator(typename LinkedList<TEntry<U, V>>::template Iterator<TEntry<U, V>> iter) : it(iter) {}

		Iterator& operator++() { ++it; return *this; }
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
		Iterator& operator--() { --it; return *this; }
		Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }

		bool operator==(const Iterator& other) const { return it == other.it; }
		bool operator!=(const Iterator& other) const { return !(*this == other); }

		U& operator*() const { return it->first; }
		U* operator->() const { return &it->first; }
	};
	Iterator<K> begin() { return data.begin(); }
	Iterator<K> end() { return data.end(); }
	Iterator<const K> begin() const { return data.begin(); }
	Iterator<const K> end()  const { return data.end(); }
	Iterator<const K> cbegin() const { return data.cbegin(); }
	Iterator<const K> cend() const { return data.cend(); }

	friend ostream& operator<<(ostream& os, const ListTable t) {
		os << "№\t" << "Ключ:\t" << "Значение:\n";
		size_t i = 1;
		for (auto& a : t.data) {
			os << i << '\t' << a.first << '\t' << a.second << "\n";
			i++;
		}
		return os;
	}
	friend void swap(ListTable<K, V>& lhs, ListTable<K, V>& rhs) {
		std::swap(lhs.data, rhs.data);
	}
};

template<class K, class V>
void ListTable<K, V>::print(ostream& os) {
	os << "№\t" << "Ключ:\t" << "Значение:" << endl;
	size_t i = 1;
	for (auto& a : data) {
		os << i << '\t' << a.first << '\t' << a.second << endl;
		i++;
	}
}

template<class K, class V>
ListTable<K, V>::ListTable() {}

template<class K, class V>
ListTable<K, V>::ListTable(initializer_list<Entry> list) {
	for (auto& a : list) add(a);
}

template<class K, class V>
ListTable<K, V>::ListTable(const ListTable& table) { 
	for (auto& elem : table.data) data.push_back(elem);
}

template<class K, class V>
ListTable<K, V>::ListTable(ListTable&& table) {
	data = table.data;
	while (table.data.get_size() > 0) table.data.pop_back();
}

template<class K, class V>
size_t ListTable<K, V>::get_size() const { return data.get_size(); }

template<class K, class V>
bool ListTable<K, V>::empty() const noexcept { return data.empty(); }

template<class K, class V>
bool ListTable<K, V>::include(const K& key) const { return find(key) != -1; }

template<class K, class V>
void ListTable<K, V>::add(const Entry& elem) { 
	if (find(elem.first) != -1) throw runtime_error("key already used");
	data.push_back(elem);
}

template<class K, class V>
void ListTable<K, V>::add(initializer_list<Entry> list) {
	for (auto& a : list) add(a);
}

template<class K, class V>
void ListTable<K, V>::remove(const K& key) {
	data.erase(find(key));
}

template<class K, class V>
void ListTable<K, V>::remove(initializer_list<K> list) { for (auto& el : list) remove(el); }

template<class K, class V>
int ListTable<K, V>::find(const K& key) const {
	int res = 0;
	for (auto& a : data) {
		if (a.first == key)
			return res;
		res++;
	}
	return -1;
}

template<class K, class V>
void ListTable<K, V>::emplace(const K& key, const V& value) { add(std::make_pair(key, value)); }

template<class K, class V>
void ListTable<K, V>::replace(const K& key, const V& value) { operator[](key) = value; }

template<class K, class V>
void ListTable<K, V>::operator=(const ListTable& table) {
	if(*this != table)
		data = table.data;
	return *this;
}

template<class K, class V>
void ListTable<K, V>::operator=(ListTable&& table) {
	if (*this != table)
		data = table.data;
	while (table.data.size > 0) table.data.pop_front();
	return *this;
}

template<class K, class V>
const V& ListTable<K, V>::operator[](const K& key) const {
	int id = find(key);
	if (id == -1) throw runtime_error("no elements with this key");
	return data[id].second;
}

template<class K, class V>
V& ListTable<K, V>::operator[](const K& key)  {
	int id = find(key);
	if (id == -1) throw runtime_error("no elements with this key");
	return data[id].second;
}

template<class K, class V>
bool ListTable<K, V>::operator==(const ListTable<K, V> table) const {	return data == table.data; }

template<class K, class V>
bool ListTable<K, V>::operator!=(const ListTable<K, V> table) const { return data != table.data; }









#endif