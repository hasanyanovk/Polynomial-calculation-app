#ifndef __SORTED_ARRAY_TABLE__
#define __SORTED_ARRAY_TABLE__

#include "table.h"

using namespace std;

template<class K, class V>
class SortedArrayTable  : public virtual Table<K, V> {
	template<class Key, class Value>
	using TEntry = pair<Key, Value>;

	using Entry = TEntry<K, V>;

	vector<Entry> data;
	enum sizes : size_t { default_size = 8 };
protected:
	void print(ostream& os) override;
public:
	SortedArrayTable();
	SortedArrayTable(size_t s);
	SortedArrayTable(const SortedArrayTable& table);
	SortedArrayTable(initializer_list<Entry> list);
	SortedArrayTable(SortedArrayTable&& table) noexcept;
	void add(const Entry& elem) override;
	void add(initializer_list<Entry> list) override;
	void remove(const K& key) override;
	void remove(initializer_list<K> list) override;
	void emplace(const K& key, const V& value);
	void replace(const K& key, const V& value);
	int find(const K& key) const override;
	bool include(const K& key) const override;
	size_t get_size() const override;
	bool empty() const;

	void operator=(const SortedArrayTable& table);
	void operator=(SortedArrayTable&& table);
	const V& operator[](const K& key) const override;
	V& operator[](const K& key) override;
	bool operator==(const SortedArrayTable<K, V> table) const;
	bool operator!=(const SortedArrayTable<K, V> table) const;

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
	Iterator<const K> begin() const { return data.begin(); }
	Iterator<const K> end()  const { return data.end(); }
	Iterator<const K> cbegin() const { return data.cbegin(); }
	Iterator<const K> cend() const { return data.cend(); }

	friend ostream& operator<<(ostream& os, const SortedArrayTable t) {
		os << "№\t" << "Ключ:\t" << "Значение:\n" ;
		size_t i = 1;
		for (auto& a : t.data) {
			os << i << '\t' << a.first << '\t' << a.second << "\n";
			i++;
		}
		return os;
	}
	friend void swap(SortedArrayTable<K, V>& lhs, SortedArrayTable<K, V>& rhs) {std::swap(lhs.data, rhs.data);}
};


template<class K, class V>
void SortedArrayTable<K, V>::print(ostream& os) {
	os << "№\t" << "Ключ:\t" << "Значение:" << endl;
	size_t i = 1;
	for (auto& a : data) {
		os << i << '\t' << a.first << '\t' << a.second << endl;
		i++;
	}
}

template<class K, class V>
SortedArrayTable<K, V>::SortedArrayTable() { data.reserve(default_size); }

template<class K, class V>
SortedArrayTable<K, V>::SortedArrayTable(size_t s) {
	if (s <= 0) throw invalid_argument("invalid capacity");
	data.reserve(s);
}

template<class K, class V>
SortedArrayTable<K, V>::SortedArrayTable(const SortedArrayTable& table) : data(table.data) {}

template<class K, class V>
inline SortedArrayTable<K, V>::SortedArrayTable(initializer_list<Entry> list) {for(auto& el : list) add(el);}

template<class K, class V>
SortedArrayTable<K, V>::SortedArrayTable(SortedArrayTable&& table) noexcept : data(std::move(table.data)) {}

template<class K, class V>
void SortedArrayTable<K, V>::add(const Entry& elem) {
	if (find(elem.first) != -1) throw runtime_error("key already used");
	for (int i = 0; i < data.size(); i++) {
		if (data[i].first > elem.first) {
			data.insert(data.begin() + i, elem);
			return;
		}
	}
	data.push_back(elem);
}

template<class K, class V>
void SortedArrayTable<K, V>::add(initializer_list<Entry> list) {
	for (auto& key : list) add(key);
}

template<class K, class V>
void SortedArrayTable<K, V>::remove(const K& key) {
	int res = find(key);
	if(res != -1)
		data.erase(data.begin() + res);
}

template<class K, class V>
void SortedArrayTable<K, V>::remove(initializer_list<K> list) {
	for (auto& key : list)
		remove(key);
}

template<class K, class V>
void SortedArrayTable<K, V>::emplace(const K& key, const V& value) { add(std::make_pair(key, value)); }

template<class K, class V>
void SortedArrayTable<K, V>::replace(const K& key, const V& value) { operator[](key) = value; }

template<class K, class V>
int SortedArrayTable<K, V>::find(const K& key) const {
	if (empty()) return -1;
	int mid = 0, left = 0, right = data.size() - 1;
	while (right > left) {
		mid = (left + right) / 2;
		if (key < data[mid].first) right = mid - 1;
		else if (key > data[mid].first) left = mid + 1;
		else return mid;
	} 
	if (data[left].first == key) return left;
	else return -1;
}

template<class K, class V>
bool SortedArrayTable<K, V>::include(const K& key) const { return find(key) != -1; }

template<class K, class V>
size_t SortedArrayTable<K, V>::get_size() const { return data.size(); }

template<class K, class V>
bool SortedArrayTable<K, V>::empty() const { return data.size() == 0; }

template<class K, class V>
void SortedArrayTable<K, V>::operator=(const SortedArrayTable& table) {
	std::copy(table.data.begin(), table.data.end(), std::back_inserter(data));
}

template<class K, class V>
void SortedArrayTable<K, V>::operator=(SortedArrayTable&& table) {
	std::copy(table.data.begin(), table.data.end(), std::back_inserter(data));
	table.data.clear();
	table.data.shrink_to_fit();
}

template<class K, class V>
const V& SortedArrayTable<K, V>::operator[](const K& key) const {
	int id = find(key);
	if (id == -1) throw runtime_error("no elements with this key");
	return data[id].second;
}

template<class K, class V>
V& SortedArrayTable<K, V>::operator[](const K& key) {
	int id = find(key);
	if (id == -1) throw runtime_error("no elements with this key");
	return data[id].second;
}

template<class K, class V>
bool SortedArrayTable<K, V>::operator==(const SortedArrayTable<K, V> table) const {
	if (get_size() != table.get_size()) return false;
	for (int i = 0; i < get_size(); i++)
		if (data[i] != table.data[i]) return false;
	return true;
}

template<class K, class V>
bool SortedArrayTable<K, V>::operator!=(const SortedArrayTable<K, V> table) const {
	return !(*this == table);
}
#endif