#ifndef __Polynom_H__
#define __Polynom_H__

#include <vector>
#include <regex>
#include <map>
#include "sorted_list.h"
#include "monom.h"
#include <functional>
#include <iomanip>


using namespace std;

struct Point {
	double x, y, z;
	friend istream& operator>>(istream& is, Point& p) {
		is >> p.x >> p.y >> p.z;
		return is;
	}

	friend ostream& operator<<(ostream& os, const Point& p) {
		os << p.x;
		os << " ";
		os << p.y;
		os << " ";
		os << p.z;
		return os;
	}

	string str() {
		return string{"(" + to_string(x) + ";" + to_string(y) + ";" + to_string(z) + ")"};
	}
};



class Polynom {

	SortedList<Monom> monoms;
	void parse_polynom(const string& str);
	string erase_spaces(const string& str);
	
public:
	                                                       // операции
	static map<string, function<Polynom(const Polynom&, const Polynom&)> > binary_operations;
	static map<string, function<Polynom(const Polynom&)> > unary_operations;
	static map<string, function<double(const Polynom&, Point&)> > ternary_operations;
	static map<string, function<double(const Polynom&, Point&, Point&)> > integrals;
	                                                       // конструкторы    
	Polynom();
	Polynom(const string& str);
	Polynom(const Polynom& p);

	void add(const Monom& m);                             // добавить моном в полином
	double calculate(double x, double y, double z) const; // расчёт значения в точке
	double calculate(Point& p) const;                     // расчёт значения в точке
	Polynom differential(char variable) const ;           // расчёт производной
	Polynom primitive(char variable) const;               // расчёт первообразной
	bool empty() const;
	                                                      // операторы
	const LinkedList<Monom>& get_monoms() const;
	Polynom operator+(const Polynom& p)const;
	Polynom& operator+=(const Polynom& p);
	Polynom operator-(const Polynom& p)const;
	Polynom& operator-=(const Polynom& p);
	Polynom operator+(const Monom& m)const;
	Polynom& operator+=(const Monom& m);
	Polynom operator-(const Monom& m)const;
	Polynom& operator-=(const Monom& m);
	Polynom operator*(const Polynom& p)const;
	Polynom& operator*=(const Polynom& p);
	Polynom operator/(const Monom& p)const;
	Polynom& operator/=(const Monom& p);
	Polynom operator*(const Monom& m)const;
	Polynom& operator*=(const Monom& m);
	Polynom operator*(const double& k)const;
	Polynom& operator*=(const double& k); 
	Polynom operator/(const Polynom& p)const;
	Polynom& operator/=(const Polynom& p);
	Polynom& operator=(const Polynom& p);

	bool operator==(const Polynom& p)const;
	bool operator!=(const Polynom& p)const;
	bool operator>(const Polynom& p)const;
	bool operator>=(const Polynom& p)const;
	bool operator<(const Polynom& p)const;
	bool operator<=(const Polynom& p)const;
	friend ostream& operator<<(ostream& os, const Polynom& p);
};

#endif 


