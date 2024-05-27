#ifndef __HASH_LIST_TABLE__
#define __HASH_LIST_TABLE__

using namespace std;

#include "linked_list.h"
#include "table.h"


template<class K, class V>
class HashListTable : public virtual Table<K, V> {
	template<class Key, class Value>
	using TEntry = pair<Key, Value>;
	using Entry = TEntry<K, V>;

	enum properties : size_t { defaul_capacity = 50, default_hash_coefficient = 1 };

	size_t hash_coefficient;
	vector<LinkedList<Entry>> data;
	size_t capacity;
	size_t size;
protected:
	void print(ostream& os) override;
	void rehash();
public:
	HashListTable();
	HashListTable(size_t capacity);
	HashListTable(size_t capacity, size_t coef);
	HashListTable(const HashListTable& table);
	HashListTable(HashListTable&& table) noexcept;

	inline size_t hash(const K& key) const;
	
	void add(const Entry& elem) override;
	void add(initializer_list<Entry> list) override;
	void remove(const K& key) override;
	void remove(initializer_list<K> list) override;
	int find(const K& key) const override;
	int find(size_t hs, const K& key) const;
	bool include(const K& key) const override;

	const V& operator[](const K& key) const override;
	V& operator[](const K& key) override;
	bool operator==(const HashListTable& table) const;
	bool operator!=(const HashListTable& table) const;
	HashListTable<K, V>& operator=(const HashListTable& table);
	HashListTable<K, V>& operator=(HashListTable&& table) noexcept;

	const size_t get_capacity() const;
	size_t get_size() const override;

	friend void swap(HashListTable<K, V>& lhs, HashListTable<K, V>& rhs) {
		swap(lhs.hash_coefficient, rhs.hash_coefficient);
		swap(lhs.capacity, rhs.capacity);
		swap(lhs.size, rhs.size);
		swap(lhs.data, rhs.data);
	}

	template<class U>
	class Iterator {
		friend class HashListTable;

		HashListTable& table;
		typename vector<LinkedList<TEntry<U, V>>>::iterator IT;
		typename LinkedList<TEntry<U, V>>::template Iterator<TEntry<U, V>> it_list;
		typename LinkedList<TEntry<U, V>>::template Iterator<TEntry<U, V>> it_list_end;

		static typename vector<LinkedList<TEntry<U, V>>>::iterator prepare(typename vector<LinkedList<TEntry<U, V>>>::iterator IT) {
			while (IT->empty()) IT++;
			return IT;
		}

		Iterator(HashListTable<U, V>& t, bool is_end)
			: table(t)
			, IT(--table.data.end())
			, it_list(IT->end())
			, it_list_end(IT->end()) {}
	public:
		Iterator(HashListTable<U, V>& t)
			: table(t)
			, IT(prepare(table.data.begin()))
			, it_list(IT->begin())
			, it_list_end(IT->end()) {}

		Iterator& operator++() {
			++it_list;
			while (it_list == IT->end() && ++IT != table.data.end()) {
				it_list = IT->begin();
				it_list_end = IT->end();
			}
			return *this; 
		}

		Iterator operator++(int) {
			Iterator<K> tmp = *this;
			++(*this);
			return tmp;
		}

		bool operator!=(const Iterator& other) const { return it_list != other.it_list; }
		bool operator==(const Iterator& other) const { return it_list == other.it_list; }

		U& operator*() const { return it_list->first; }
		U* operator->() const { return &it_list->first; }
	};
	Iterator<K> begin() { return Iterator<K>(*this); }
	Iterator<K> end() { return Iterator<K>(*this, true); }
	Iterator<const K> begin() const { return Iterator<K>(*this); }
	Iterator<const K> end()  const { return Iterator<K>(*this, true); }
	Iterator<const K> cbegin() const { return Iterator<K>(*this); }
	Iterator<const K> cend() const { return Iterator<K>(*this, true); }

	friend ostream& operator<<(ostream& os, HashListTable<K, V>& t) {
		os << "№\t" << "Ключ:\t" << "Значение:\n";
		size_t i = 1;
		for (auto& a : t) {
			os << i << '\t' << a << '\t' << t[a] << "\n";
			i++;
		}
		return os;
	}
}; 

template<class K, class V>
void HashListTable<K, V>::print(ostream& os) {
	os << "№\t\t" << "Ключ:\t\t" << "Значение:" << endl;
	size_t i = 1;
	for (auto& a : data) 
		for(auto& d : a) {
			os << i << "\t\t" << d.first << "\t\t" << d.second << endl;
			i++;
		}
}

template<class K, class V>
void HashListTable<K, V>::rehash() {
	HashListTable<K, V> tmp(capacity * 2, hash_coefficient * 2);
	for (auto& a : data)
		for (auto& p : a)
			tmp.add(p);
	*this = std::move(tmp);

}

template<class K, class V>
inline HashListTable<K, V>::HashListTable() : HashListTable(defaul_capacity, default_hash_coefficient) {}

template<class K, class V>
inline HashListTable<K, V>::HashListTable(size_t capacity) : HashListTable(capacity, default_hash_coefficient) {}

template<class K, class V>
inline HashListTable<K, V>::HashListTable(size_t capacity, size_t coef) : hash_coefficient(coef), size(0) {
	if (capacity <= 0) throw invalid_argument("size should be greater than zero");
	data.resize(capacity);
	this->capacity = data.capacity();
}

template<class K, class V>
inline HashListTable<K, V>::HashListTable(const HashListTable& table) : 
	hash_coefficient(table.hash_coefficient), 
	capacity(table.capacity),
	size(table.size), data(table.data) {}

template<class K, class V>
inline HashListTable<K, V>::HashListTable(HashListTable&& table) noexcept 
	: size(std::move(table.size))
	,capacity(std::move(table.capacity))
	,hash_coefficient(std::move(table.hash_coefficient))
	,data(std::move(table.data)) 
	{table.size = 0;}

template<class K, class V>
inline size_t HashListTable<K, V>::hash(const K& key) const { 
	return std::hash<K>{hash_coefficient}(key) % capacity; }


template<class K, class V>
inline void HashListTable<K, V>::add(const Entry& elem) {
	if (find(elem.first) != -1) throw runtime_error("key already used");
	if (size >= capacity / 2) rehash();
	data[hash(elem.first)].push_back(elem);
	size++;
}

template<class K, class V>
inline void HashListTable<K, V>::add(initializer_list<Entry> list) { for (auto& el : list) add(el); }

template<class K, class V>
inline void HashListTable<K, V>::remove(const K& key) {
	size_t h = hash(key);
	int id = find(h, key);
	if (id == -1) return;
	data[h].erase(id);
	size--;
}

template<class K, class V>
inline void HashListTable<K, V>::remove(initializer_list<K> list) { for (auto& el : list) remove(el); }

template<class K, class V>
inline int HashListTable<K, V>::find(const K& key) const {return find(hash(key), key);}

template<class K, class V>
inline int HashListTable<K, V>::find(size_t id, const K& key) const {
	int i = 0;
	if (id < 0 || id > capacity) throw out_of_range("invalid index");
	if (data[id].empty()) return -1;
	for (auto& a : data[id]) {
		if (a.first == key) return i;
		i++;
	}
	return -1;
}

template<class K, class V>
bool HashListTable<K, V>::include(const K& key) const { return find(key) != -1; }

template<class K, class V>
const V& HashListTable<K, V>::operator[](const K& key) const {
	size_t h = hash(key);
	int f = find(h, key);
	if (f == -1) throw runtime_error("no elements with this key");
	return data[h][f].second;
}

template<class K, class V>
V& HashListTable<K, V>::operator[](const K& key)  {
	size_t h = hash(key);
	int f = find(h, key);
	if (f == -1) throw runtime_error("no elements with this key");
	return data[h][f].second;
}

template<class K, class V>
bool HashListTable<K, V>::operator==(const HashListTable& table) const { return data == table.data; }

template<class K, class V>
bool HashListTable<K, V>::operator!=(const HashListTable& table) const { return !(*this == table); }

template<class K, class V>
HashListTable<K, V>& HashListTable<K, V>::operator=(const HashListTable& table) {
	hash_coefficient = table.hash_coefficient;
	capacity = table.capacity;
	size = table.size;
	data = table.data;
	return *this;
}

template<class K, class V>
HashListTable<K, V>& HashListTable<K, V>::operator=(HashListTable&& table) noexcept {
	hash_coefficient = move(table.hash_coefficient);
	capacity = move(table.capacity);
	size = move(table.size);
	data = move(table.data);
	table.size = 0;
	return *this;
}

template<class K, class V>
inline const size_t HashListTable<K, V>::get_capacity() const { return capacity; }

template<class K, class V>
inline size_t HashListTable<K, V>::get_size() const { return size; }

















#endif 


