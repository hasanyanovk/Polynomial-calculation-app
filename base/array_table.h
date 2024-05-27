#ifndef __ARRAY_TABLE_H__
#define __ARRAY_TABLE_H__

#include "table.h"


template<class K, class V>
class ArrayTable : public virtual Table<K,V> {
	template<class Key, class Value>
	using TEntry = pair<Key, Value>;

	using Entry = TEntry<K, V>;

	vector<Entry> data;
	enum sizes : size_t { default_size = 8 };
protected:
	void print(ostream& os) override;
public:
	ArrayTable();
	ArrayTable(size_t s);
	ArrayTable(const ArrayTable& arr);
	ArrayTable(initializer_list<Entry> list);
	ArrayTable(ArrayTable&& arr) noexcept;

	void add (const Entry& elem) override;
	void add(initializer_list<Entry> list) override;
	void remove(const K& key) override;
	void remove(initializer_list<K> list) override;
	int find(const K& key) const override;
	void emplace(const K& key, const V& value);
	void replace(const K& key, const V& value);
	void clear();
	size_t get_size() const override;
	bool empty() const;
	bool include(const K& key) const override;

	void operator=(const ArrayTable& table);
	void operator=(ArrayTable&& table);
	const V& operator[](const K& key) const override;
	V& operator[](const K& key) override;
	bool operator==(const ArrayTable<K, V> table) const;
	bool operator!=(const ArrayTable<K, V> table) const;

	template<class U>
	class Iterator {
		typename vector<TEntry<U, V>>::iterator it;

	public:
		Iterator(typename vector<TEntry<U, V>>::iterator iter) : it(iter) {}

		Iterator& operator++() { ++it; return *this; }
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
		Iterator& operator--() { --it; return *this; }
		Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }

		bool operator==(const Iterator& other) const { return it == other.it; }
		bool operator!=(const Iterator& other) const { return !(*this == other); }

		U& operator*() const { return it->first; }
		U* operator->() const { return &it->first; }
	};
	Iterator<K> begin() { return Iterator<K>(data.begin()); }
	Iterator<K> end() { return Iterator<K>(data.end()); }
	Iterator<const K> begin() const { return Iterator<K>(data.begin()); }
	Iterator<const K> end()  const { return Iterator<K>(data.end()); }
	Iterator<const K> cbegin() const { return Iterator<K>(data.cbegin()); }
	Iterator<const K> cend() const { return Iterator<K>(data.cend()); }

	friend ostream& operator<<(ostream& os, const ArrayTable& t) {
		os << "№\t" << "Ключ:\t" << "Значение:\n" ;
		size_t i = 1;
		for (auto& a : t.data) {
			cout << i << '\t' << a.first << '\t' << a.second << "\n";
			i++;
		}
		return os;
	}
	friend void swap(ArrayTable<K, V>& lhs, ArrayTable<K, V>& rhs) {std::swap(lhs.data, rhs.data); }
};


template<class K, class V>
void ArrayTable<K, V>::print(ostream& os) {
	os << "№\t" << "Ключ:\t" << "Значение:\n";
	size_t i = 1;
	for (auto& a : data) {
		os << i << '\t' << a.first << '\t' << a.second << "\n";
		i++;
	}
}

template<class K, class V>
ArrayTable<K, V>::ArrayTable() { data.reserve(default_size); }

template<class K, class V>
ArrayTable<K, V>::ArrayTable(size_t s) {
	if (s <= 0) throw invalid_argument("invalid capacity");
	data.reserve(s);
}

template<class K, class V>
ArrayTable<K, V>::ArrayTable(const ArrayTable& arr) {
	std::copy(arr.data.begin(), arr.data.end(), std::back_inserter(data));
}

template<class K, class V>
inline ArrayTable<K, V>::ArrayTable(initializer_list<Entry> list) {
	std::copy(list.begin(), list.end(), std::back_inserter(data));
}

template<class K, class V>
ArrayTable<K, V>::ArrayTable(ArrayTable&& arr) noexcept : data(move(arr.data)) {}

template<class K, class V>
void ArrayTable<K, V>::add (const Entry& elem) {
	if (find(elem.first) != -1) throw runtime_error("key already used");
	data.push_back(elem);
}

template<class K, class V>
void ArrayTable<K, V>::add(initializer_list<Entry> list) {
	for (auto& key : list) add(key);
}

template<class K, class V>
void ArrayTable<K, V>::remove(const K& key) {
	for (int i = 0; i < data.size(); i++)
		if (data[i].first == key)
			data.erase(data.begin() + i);
}

template<class K, class V>
void ArrayTable<K, V>::remove(initializer_list<K> list) {
	auto b = data.begin();
	for (auto& key : list) {
		int res = find(key);
		if (res != -1) data.erase(b + res);
	}
}

template<class K, class V>
int ArrayTable<K, V>::find(const K& key) const {
	for (int i = 0; i < data.size(); i++)
		if (data[i].first == key)
			return i;
	return -1;
}

template<class K, class V>
void ArrayTable<K, V>::emplace(const K& key, const V& value) { add(std::make_pair(key, value)); }


template<class K, class V>
size_t ArrayTable<K, V>::get_size() const { return data.size(); }

template<class K, class V>
bool ArrayTable<K, V>::empty() const { return data.size() == 0; }

template<class K, class V>
bool ArrayTable<K, V>::include(const K& key) const { return find(key) != -1; }

template<class K, class V>
void ArrayTable<K, V>::operator=(const ArrayTable& table) {
	std::copy(table.data.begin(), table.data.end(), std::back_inserter(data));
}

template<class K, class V>
void ArrayTable<K, V>::operator=(ArrayTable&& table) {
	std::copy(table.data.begin(), table.data.end(), std::back_inserter(data));
	table.data.clear();
	table.data.shrink_to_fit();
}

template<class K, class V>
const V& ArrayTable<K, V>::operator[](const K& key) const {
	int id = find(key);
	if (id == -1) throw runtime_error("no elements with this key");
	return data[id].second;
}

template<class K, class V>
V& ArrayTable<K, V>::operator[](const K& key) {
	int id = find(key);
	if (id == -1) throw runtime_error("no elements with this key");
	return data[id].second;
}

template<class K, class V>
void ArrayTable<K, V>::replace(const K& key, const V& value) {
	operator[](key) = value;
}

template<class K, class V>
void ArrayTable<K, V>::clear() { data.clear(); }

template<class K, class V>
bool ArrayTable<K, V>::operator==(const ArrayTable<K, V> table) const {
	if (get_size() != table.get_size()) return false;
	for (int i = 0; i < get_size(); i++)
		if (data[i] != table.data[i]) return false;
	return true;
}

template<class K, class V>
bool ArrayTable<K, V>::operator!=(const ArrayTable<K, V> table) const {
	return !(*this == table);
}



#endif