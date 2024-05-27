#ifndef __HASH_ARRAY_TABLE_H__
#define __HASH_ARRAY_TABLE_H__

#include "table.h"

template<class K,class V>
class HashArrayTable : public virtual Table<K, V> {
	
	template<class Key, class Value>
	using TEntry = pair<Key, Value>;
	using Entry = TEntry<K, V>;
public:
	enum class Status { Free, Delete, Busy }; // Free is a default
private:
	enum properties : size_t { default_capacity = 500, default_hash_coefficient = 1, step = 7 };

	using HEntry = pair<Status, Entry>;  // Default structure have papameters: (Free, <"",0>) 
public:
	size_t hash_coefficient;
	vector<HEntry> data;
	size_t capacity;
	size_t size;
protected:
	void print(ostream& os) override;
	void rehash();
	HashArrayTable(const size_t& capacity, const size_t coef);
public:
	HashArrayTable();
	HashArrayTable(const size_t& capacity);
	HashArrayTable(const HashArrayTable& table);
	HashArrayTable(HashArrayTable&& table);

	void add(const Entry& elem) override;
	void add(initializer_list<Entry> list) override;
	void remove(const K& key) override;
	void remove(initializer_list<K> list) override;
	int find(const K& key) const override;
	bool include(const K& key) const override;

	inline size_t hash(const K& key) const;

	const V& operator[](const K& key) const override;
	V& operator[](const K& key) override;
	bool operator==(const HashArrayTable& table) const;
	bool operator!=(const HashArrayTable& table) const;
	HashArrayTable& operator=(const HashArrayTable& table);
	HashArrayTable& operator=(HashArrayTable&& table);

	const size_t get_capacity() const;
	size_t get_size() const override;

	friend void swap(HashArrayTable<K, V>& lhs, HashArrayTable<K, V>& rhs) {
		swap(lhs.hash_coefficient, rhs.hash_coefficient);
		swap(lhs.capacity, rhs.capacity);
		swap(lhs.size, rhs.size);
		swap(lhs.data, rhs.data);
	}

	friend ostream& operator<<(ostream& os, HashArrayTable<K, V>& t) {
		os << "№\t" << "Ключ:\t" << "Значение:\n";
		size_t i = 1;
		for (auto& a : t) {
			os << i << '\t' << a << '\t' << t[a] << "\n";
			i++;
		}
		return os;
	}

	template<class U>
	class Iterator {
		friend class HashArrayTable;
		typename vector<pair<Status, pair<U, V>>>::iterator it;
		typename vector<pair<Status, pair<U, V>>>::iterator it_end;
	public:
		Iterator(typename vector<pair<Status, pair<U, V>>>::iterator iter, typename vector<pair<Status, pair<U, V>>>::iterator iter_end)
			: it(iter), it_end(iter_end) {
			while (it != it_end && it->first != Status::Busy) ++it;
		}

		Iterator& operator++() {
			++it;
			while (it != it_end && it->first != Status::Busy) ++it;
			return *this;
		}

		Iterator& operator--() {
			if (it == data.begin()) return *this;
			--it;
			while (it != data.begin() && it->first != Status::Busy) --it;
			return *this;
		}

		bool operator==(const Iterator& other) { return it == other.it; }
		bool operator!=(const Iterator& other) { return it != other.it; }

		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
		Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }

		U* operator->() { return &it->second.first; }
		U& operator*() { return it->second.first; }
	};
	Iterator<K> begin() { return Iterator<K>(data.begin(), data.end()); }
	Iterator<K> end() { return Iterator<K>(data.end(), data.end()); }
	Iterator<const K> begin() const { return Iterator<K>(data.begin(), data.end()); }
	Iterator<const K> end()  const { return Iterator<K>(data.end(), data.end()); }
	Iterator<const K> cbegin() const { return Iterator<K>(data.cbegin(), data.cend()); }
	Iterator<const K> cend() const { return Iterator<K>(data.cend(), data.cend()); }
};

template<class K, class V>
HashArrayTable<K, V>::HashArrayTable() : HashArrayTable(default_capacity, default_hash_coefficient) {}

template<class K, class V>
HashArrayTable<K, V>::HashArrayTable(const size_t& capacity) : HashArrayTable(capacity, default_hash_coefficient) {}

template<class K, class V>
HashArrayTable<K, V>::HashArrayTable(const size_t& capacity, const size_t coef)
	: hash_coefficient(coef)
	, capacity(capacity)
	, size(0) {
	if (capacity <= 0) throw invalid_argument("invalid capacity");
	data.resize(capacity);
}

template<class K, class V>
inline HashArrayTable<K, V>::HashArrayTable(const HashArrayTable& table) :
	hash_coefficient(table.hash_coefficient),
	capacity(table.capacity),
	size(table.size), data(table.data) {}

template<class K, class V>
HashArrayTable<K, V>::HashArrayTable(HashArrayTable&& table)
	: hash_coefficient(move(table.hash_coefficient))
	, capacity(move(table.capacity))
	, size(move(table.size)) 
	, data(move(table.data)) 
	  {table.size = 0;}

template<class K, class V>
void HashArrayTable<K, V>::print(ostream& os) {
	os << "№\t" << "Ключ:\t" << "Значение:" << endl;
	size_t i = 1;
	for (auto& a : *this) {
		os << i << '\t' << a << '\t' << operator[](a) << endl;
		i++;
	}
}

template<class K, class V>
inline void HashArrayTable<K, V>::rehash() {
	HashArrayTable<K, V> tmp(capacity * 2, hash_coefficient * 2);
	for (auto& a : *this) tmp.add({a, operator[](a)});
	swap(*this, tmp);
}

template<class K, class V>
void HashArrayTable<K, V>::add(const Entry& elem) {
	if (find(elem.first) != -1) throw logic_error("key already used");
	if (size >= 0.7 * capacity) rehash();
	size_t h = hash(elem.first);
	while (data[h].first != Status::Free) h = (h + step) % capacity;
	data[h] = { Status::Busy, elem };
	size++;
}

template<class K, class V>
void HashArrayTable<K, V>::add(initializer_list<Entry> list) { for (const auto& el : list) add( el ); }

template<class K, class V>
void HashArrayTable<K, V>::remove(const K& key) {
	int id = find(key);
	if (id == -1) return;
	data[id] = HEntry{ Status::Delete, Entry{} };
	size--;
}

template<class K, class V>
void HashArrayTable<K, V>::remove(initializer_list<K> list) { for (auto& el : list) remove(el); }

template<class K, class V>
int HashArrayTable<K, V>::find(const K& key) const {
	size_t id = hash(key), start_id = id;
	while (data[id].first == Status::Busy) {
		if (data[id].second.first == key) return id;
		id = (id + step) % capacity;
		if (id == start_id) break;
	}
	return -1;
}

template<class K, class V>
bool HashArrayTable<K, V>::include(const K& key) const { return find(key) != -1; }


template<class K, class V>
inline size_t HashArrayTable<K, V>::hash(const K& key) const { return std::hash<K>{hash_coefficient}(key) % capacity; }

template<class K, class V>
const V& HashArrayTable<K, V>::operator[](const K& key) const {
	int id = find(key);
	if (id < 0) throw logic_error("no elements with this key");
	return data[id].second.second;
}

template<class K, class V>
inline V& HashArrayTable<K, V>::operator[](const K& key) {
	int id = find(key);
	if (id < 0) throw logic_error("no elements with this key");
	return data[id].second.second;
}

template<class K, class V>
inline bool HashArrayTable<K, V>::operator==(const HashArrayTable& table) const { return data == table.data; }

template<class K, class V>
inline bool HashArrayTable<K, V>::operator!=(const HashArrayTable& table) const { return data != table.data; }

template<class K, class V>
HashArrayTable<K, V>& HashArrayTable<K, V>::operator=(const HashArrayTable& table) {
	capacity = table.capacity;
	size = table.size;
	data = table.data;
	return *this;
}

template<class K, class V>
HashArrayTable<K,V>& HashArrayTable<K, V>::operator=(HashArrayTable&& table) {
	capacity = move(table.capacity);
	size = move(table.size);
	data = move(table.data);
	table.size = 0;
	return *this;
}

template<class K, class V>
inline const size_t HashArrayTable<K, V>::get_capacity() const { return capacity; }

template<class K, class V>
inline  size_t HashArrayTable<K, V>::get_size() const { return size; }

#endif