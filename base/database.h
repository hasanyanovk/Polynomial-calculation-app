#ifndef __DATABASE_H__
#define __DATABASE_H__

#include"array_table.h"
#include"list_table.h"
#include"sorted_array_table.h"
#include"hash_array_table.h"
#include"hash_list_table.h"
#include"tree_table.h"
#include<fstream>

template <class Key, class Value>
class Database {
	enum class tables : short { 
		ArrayTable = 0, 
		ListTable = 1, 
		SortedArrayTable = 2, 
		HashArrayTable = 3,
		HashListTable = 4, 
		TreeTable = 5
	};
	using Entry = std::pair<Key, Value>;
	vector<Table<Key, Value>*> data;
	short active_table;
public:
	Database();
	~Database();
	void add(const Entry& elem);
	void add(initializer_list<Entry> list);
	void emplace(const Key& key, const Value& value);
	void replace(const Key& key, const Value& value);
	void remove(const Key& elem);
	void remove(initializer_list<Key> list);
	bool empty() const;
	size_t size() const;
	void set_active(const short& table);
	void set_active(const string& table);
	bool include(const Key& key) const;

	const Value& operator[](const Key& key) const;
	Value& operator[](const Key& key);
	friend ostream& operator<<(ostream& os, const Database<Key, Value>& db) {
		os << static_cast<tables>(db.active_table) << ":" << endl;
		os << *db.data[db.active_table];
		return os;
	}

	friend ofstream& operator>>(ofstream& of, const Database<Key, Value>& db) {
		if (!of.is_open()) throw runtime_error("File can`t be open");
		of << static_cast<tables>(db.active_table) << ":" << endl;
		of << *db.data[db.active_table];
		return of;
	}

	friend ostream& operator<<(ostream& os, tables t) {
		switch (t) {
		case tables::ArrayTable :return os << "ArrayTable";
		case tables::ListTable :return os << "ListTable";
		case tables::SortedArrayTable :return os << "SortedArrayTable";
		case tables::HashArrayTable:return os << "HashArrayTable";
		case tables::HashListTable:return os << "HashListTable";
		case tables::TreeTable:return os << "TreeTable";
		default: return os << static_cast<short>(t);
		}
	}
};

template<class Key, class Value>
inline Database<Key, Value>::Database() : active_table(0) {
	data.push_back(new ArrayTable<Key, Value>);
	data.push_back(new ListTable<Key, Value>);
	data.push_back(new SortedArrayTable<Key, Value>);
	data.push_back(new HashArrayTable<Key, Value>);
	data.push_back(new HashListTable<Key, Value>);
	data.push_back(new TreeTable<Key, Value>);
}

template<class Key, class Value>
inline Database<Key, Value>::~Database() {
	for (auto* a : data) delete a;
}

template<class Key, class Value>
void Database<Key, Value>::add(const Entry& elem) {
	for (auto* t : data) t->add(elem);
}

template<class Key, class Value>
inline void Database<Key, Value>::add(initializer_list<Entry> list) { 
	for (const auto& el : list) this->add(el); 
}

template<class Key, class Value>
void Database<Key, Value>::emplace(const Key& key, const Value& value) {
	for (auto* t : data) t->add(std::make_pair(key, value));
}

template<class Key, class Value>
void Database<Key, Value>::replace(const Key& key, const Value& value) {
	for (auto* t : data) t->operator[](key) = value;
}

template<class Key, class Value>
void Database<Key, Value>::remove(const Key& elem) { 
	for (auto* t : data) t->remove(elem); 
}

template<class Key, class Value>
void Database<Key, Value>::remove(initializer_list<Key> list) {
	for (const auto& el : list) this->remove(el);
}

template<class Key, class Value>
bool Database<Key, Value>::empty() const { return size() == 0; }

template<class Key, class Value>
size_t Database<Key, Value>::size() const { return data[active_table]->get_size(); }

template<class Key, class Value>
void Database<Key, Value>::set_active(const short& table) { active_table = table; }

template<class Key, class Value>
void Database<Key, Value>::set_active(const string& table) { 
	if (table == "ArrayTable") set_active(0);
	else if (table == "ListTable") set_active(1);
	else if (table == "SortedArrayTable") set_active(2);
	else if (table == "HashArrayTable") set_active(3);
	else if (table == "HashListTable") set_active(4);
	else if (table == "TreeTable") set_active(5);
	else throw invalid_argument("invalid name of table");
}

template<class Key, class Value>
bool Database<Key, Value>::include(const Key& key) const { return data[active_table]->include(key); }


template<class Key, class Value>
const Value& Database<Key, Value>::operator[](const Key& key) const { return data[active_table]->operator[](key); }

template<class Key, class Value>
Value& Database<Key, Value>::operator[](const Key& key) { return data[active_table]->operator[](key); }










#endif


