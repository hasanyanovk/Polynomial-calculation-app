#include "polynom.h"

Polynom::Polynom() {}

Polynom::Polynom(const string& str) { parse_polynom(erase_spaces(str)); }

Polynom::Polynom(const Polynom& p) : monoms(p.monoms) {}

string Polynom::erase_spaces(const string& s)
{
	string res = "";
	for (auto& c : s)
		if (c != ' ')
			res += c;
	return res;
}

map<string, function<Polynom(const Polynom&, const Polynom&)> > Polynom::binary_operations = { 
	
	{"+", [](const Polynom& a, const Polynom& b) {return a + b; }},
	{"-", [](const Polynom& a, const Polynom& b) {return a - b; }},
	{"*", [](const Polynom& a, const Polynom& b) {return a * b; }},
	{"/", [](const Polynom& a, const Polynom& b) {return a / b; }}
};

map<string, function<Polynom(const Polynom&)> > Polynom::unary_operations = {

	{"dx", [](const Polynom& a) {return a.differential('x'); }},
	{"dy", [](const Polynom& a) {return a.differential('y'); }},
	{"dz", [](const Polynom& a) {return a.differential('z'); }},
	{"Fdx", [](const Polynom& a) {return a.primitive('x'); }},
	{"Fdy", [](const Polynom& a) {return a.primitive('y'); }},
	{"Fdz", [](const Polynom& a) {return a.primitive('z'); }},
	{"~", [](const Polynom& a) {return a* (-1); }}
};

map<string, function<double(const Polynom&, Point&)> > Polynom::ternary_operations = {
	{"=", [](const Polynom& p, Point& p1) {return p.calculate(p1); }}
};

map<string, function<double(const Polynom&,Point&, Point&)> > Polynom::integrals = {
	{"Idx",[](const Polynom& p, Point& p1,Point& p2) 
		{return p.primitive('x').calculate(p1) - p.primitive('x').calculate(p2); }},
	{"Idy",[](const Polynom& p, Point& p1,Point& p2)
		{return p.primitive('y').calculate(p1) - p.primitive('y').calculate(p2); }},
	{"Idz",[](const Polynom& p, Point& p1,Point& p2)
		{return p.primitive('z').calculate(p1) - p.primitive('z').calculate(p2); }},
};

void Polynom::parse_polynom(const string& str) {
	string tmp = str[0] == '-' ? "-" : "";
	for (const auto& c : str) {
		if (c != '+' && c != '-') {
			tmp += c;
			continue;
		}
		if (tmp.empty() || tmp == "-") continue;
		add(Monom{ tmp });
		tmp = (c == '-') ? "-" : "";
	}
	if (!tmp.empty()) add(Monom{ tmp });
}

Polynom Polynom::primitive(char variable) const {
	Polynom res;
	switch (variable) {
	case 'x':
	case 'X':
		for (const auto& m : monoms)
			res.add({ m.get_coef() / (m.get_x() + 1), m.get_x() + 1, m.get_y(), m.get_z() });
		break;
	case 'y':
	case 'Y':
		for (const auto& m : monoms)
			res.add({ m.get_coef() / (m.get_y() + 1), m.get_x(), m.get_y() + 1, m.get_z() });
		break;
	case 'z':
	case 'Z':
		for (const auto& m : monoms)
			res.add({ m.get_coef() / (m.get_z() + 1), m.get_x(), m.get_y(), m.get_z() + 1 });
		break;
	}
	return res;
}

bool Polynom::empty() const { return monoms.empty(); }

Polynom Polynom::differential(char variable) const {
	Polynom res;
	switch (variable) {
	case 'x':
		for (const auto& m : monoms)
			res.add({ m.get_coef() * m.get_x(), m.get_x() - 1, m.get_y(), m.get_z() });
		break;
	case 'y':
		for (const auto& m : monoms)
			res.add({ m.get_coef() * m.get_y(), m.get_x(), m.get_y() - 1, m.get_z() });
		break;
	case 'z':
		for (const auto& m : monoms)
			res.add({ m.get_coef() * m.get_z(), m.get_x(), m.get_y(), m.get_z() - 1 });
		break;
	}
	return res;
}

double Polynom::calculate(double x, double y, double z) const {
	if (monoms.empty()) throw logic_error("empty Polynom");
	double res = 0.0;
	for (const auto& m : monoms)
		res += m.calculate(x,y,z);
	return res;
}

double Polynom::calculate(Point& p) const{
	if (monoms.empty()) throw logic_error("empty Polynom");
	double res = 0.0;
	for (const auto& m : monoms)
		res += m.calculate(p.x, p.y, p.z);
	return res;
}

void Polynom::add(const Monom& m){
	if (m.get_coef() == 0) return;
	size_t i = 0;
	for (auto& m1 : monoms) {
		if (m1.equal(m)) {
			m1 += m;
			if (m1.get_coef() == 0)
				monoms.erase(i);
			return;
		}
		i++;
	}
	monoms.insert_in_order(m, [](Monom x, Monom y) {return x > y; });
}

const LinkedList<Monom>& Polynom::get_monoms() const { return monoms; }

Polynom Polynom::operator*(const double& k) const {
	Polynom res(*this);
	for (auto& m : res.monoms)
		m *= k;
	return res;
}

Polynom& Polynom::operator*=(const double& k) {
	for (auto& m : monoms)
		m *= k;
	return *this;
}

Polynom Polynom::operator+(const Polynom& p)const {
	Polynom res(*this);
	for (const auto& m : p.monoms)
		res.add(m);
	return res;
}

Polynom& Polynom::operator+=(const Polynom& p) {
	for (const auto& m : p.monoms)
		this->add(m);
	return *this;
}

Polynom Polynom::operator-(const Polynom& p) const {
	return Polynom{*this + (p * (-1.0))};
}

Polynom& Polynom::operator-=(const Polynom& p) {
	for (const auto& m : p.monoms)
		this->add(m * (-1.0));
	return *this;
}

Polynom Polynom::operator+(const Monom& m)const {
	Polynom res(*this);
	res.add(m);
	return res;
}

Polynom& Polynom::operator+=(const Monom& m) {
	add(m);
	return *this;
}

Polynom Polynom::operator-(const Monom& m)const {
	Polynom res(*this);
	for (auto& m1 : res.monoms)
		if (m1.equal(m)) {
			m1 -= m;
			return res;
		}
	res.add(m * (-1.0));
	return res;
}

Polynom& Polynom::operator-=(const Monom& m) {
	for (auto& m1 : monoms)
		if (m1.equal(m)){
			m1 -= m;
			return *this;
		}
	add(m * (-1.0));
	return *this;
}

Polynom Polynom::operator*(const Polynom& p) const {
	Polynom res;
	for (auto& m1 : monoms)
		for(auto& m2 : p.monoms)
			res.add(m1 * m2);
	return res;
}

Polynom& Polynom::operator*=(const Polynom& p) {
	Polynom res;
	for (auto& m1 : monoms)
		for (auto& m2 : p.monoms)
			res.add(m1 * m2);
	*this = res;
	return *this;
}

Polynom Polynom::operator*(const Monom& m) const {
	Polynom res;
	for (auto& m1: monoms)
		res.add(m1 * m);
	return res;
}

Polynom& Polynom::operator*=(const Monom& m) {
	for (auto& m1 : monoms)
		m1 *= m;
	return *this;
}

Polynom Polynom::operator/(const Monom& p) const {
	Polynom res;
	for (const auto& m : monoms)
		res.add(m / p);
	return res;
}

Polynom& Polynom::operator/=(const Monom& p) {
	for (auto& m : monoms)
		m /= p;
	return*this;
}

Polynom Polynom::operator/(const Polynom& p) const {
	if (p.monoms.get_size() == 0) throw logic_error("divide by zero");
	if (*this == p) return Polynom{ "1" };
	Polynom res, remainder(*this);
	Monom cur = p.monoms.get_front();
	while (remainder.monoms.get_size() >= p.monoms.get_size()) {
		Monom lead = remainder.monoms.get_front() / cur;
		res += lead;
		remainder -= p * lead;
	}
	return res;
}

Polynom& Polynom::operator/=(const Polynom& p) {
	*this = *this / p;
	return *this;
}

Polynom& Polynom::operator=(const Polynom& p) {
	monoms = p.monoms; 
	return *this;
}

bool Polynom::operator==(const Polynom& p) const {
	if (monoms.get_size() != p.monoms.get_size()) return false;
	for (size_t i = 0; i < monoms.get_size(); i++)
		if (monoms[i] != p.monoms[i]) return false;
	return true;
}

bool Polynom::operator!=(const Polynom& p) const {	return !(*this == p); }

bool Polynom::operator>(const Polynom& p) const { return monoms.get_front() > p.get_monoms().get_front(); }

bool Polynom::operator>=(const Polynom& p) const { return monoms.get_front() >= p.get_monoms().get_front(); }

bool Polynom::operator<(const Polynom& p) const { return monoms.get_front() < p.get_monoms().get_front(); }

bool Polynom::operator<=(const Polynom& p) const { return monoms.get_front() <= p.get_monoms().get_front(); }

ostream& operator<<(ostream& os, const Polynom& p) {
	if (p.monoms.empty()) {
		os << "[]";
		return os;
	}
	os << p.monoms.get_front();
	for (auto it = ++p.monoms.cbegin(); it != p.monoms.cend(); ++it)
		if (it->get_coef() != 0) {
			if (it->get_coef() > 0) os << "+";
			os << *it;
		}
	return os;
}