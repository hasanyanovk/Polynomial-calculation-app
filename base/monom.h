#ifndef __MONOM_H__
#define __MONOM_H__

#include <stdexcept>
#include <string>
#include <iostream>
#include <regex>

using namespace std;

class Monom {
	double k;                                            // коэффициент монома
	int degX = 0,                                        // степень x
	degY = 0,											 // степень Y
	degZ = 0;											 // степень x
	                                                     // методы для парсинга строки
	void set_coeffitient(const string& str);             
	void set_degree(const char& c, const string& degree);
	inline void match_pattern(const string& str) const;
	void parse_monom(const string& str);
public:
														 // конструкторы
	Monom();
	Monom(double K, int X, int Y, int Z);
	Monom(const Monom& m);
	Monom(const string& s);
	                                                     // геттеры
	double get_coef() const;
	int get_x() const;
	int get_y() const;
	int get_z() const;
	                                                      
	double calculate(double x, double y, double z) const;// расёт значение в точке
	bool equal(const Monom& m)const;                     // проверка на подобие
	                                                     // операторы
	bool operator<(const Monom& m) const;
	bool operator<=(const Monom& m) const;
	bool operator>(const Monom& m)const;
	bool operator>=(const Monom& m)const;
	bool operator==(const Monom& m)const;
	bool operator!=(const Monom& m)const;
	Monom operator+(const Monom& m)const;
	Monom& operator+=(const Monom& m);
	Monom operator-(const Monom& m)const;
	Monom& operator-=(const Monom& m);
	Monom operator*(const Monom& m)const;
	Monom& operator*=(const Monom& m);
	Monom operator/(const Monom& m)const;
	Monom& operator/=(const Monom& m);
	Monom operator*(const double& k)const;
	Monom& operator*=(const double& k);
	Monom operator/(const double& k)const;
	Monom& operator/=(const double& k);
	friend ostream& operator<<(ostream& os, const Monom& m);
};


#endif
