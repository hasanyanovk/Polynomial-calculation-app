#include "monom.h"
#include <cctype>

Monom::Monom() : k(0), degX(0), degY(0), degZ(0) {}

Monom::Monom(double K, int X, int Y, int Z) : k(K), degX(X), degY(Y), degZ(Z) {}

Monom::Monom(const Monom& m) : k(m.k), degX(m.degX), degY(m.degY), degZ(m.degZ) {}

inline void Monom::match_pattern(const string& str) const {
	regex pattern("([+-]?[\\d.]*)(x\\d*?)?(y\\d*?)?(z\\d*?)?");
	if (!regex_match(str, pattern)) throw invalid_argument("invalid expression");
}

void Monom::set_coeffitient(const string& str) {
	if (str.empty()) k = 1.0;
	else if (str == "-") k = -1.0;
	else {
		k = stod(str);
	}
}

void Monom::set_degree(const char& c, const string& degree) {
	int d = degree.empty() ? 1 : stoi(degree);
	switch (c) {
	case 'x':
		degX = d;
		break;
	case 'y':
		degY = d;
		break;
	case'z':
		degZ = d;
		break;
	default:
		break;
	}
}

void Monom::parse_monom(const string& str) { //-xy5
	if (str.empty()) throw invalid_argument("empty monom");
	string tmp = "";
	char last = '~';
	bool flag = 1;
	for (const auto& c : str) {
		if (!isalpha(c)) {
			tmp += c;
			continue;
		}
		flag = 0;
		switch (last) {
		case '~':
			set_coeffitient(tmp);
			break;
		default:
			set_degree(last, tmp);
			break;
		}
		
		last = c;
		tmp = "";
	}
	flag ? set_coeffitient(tmp) : set_degree(last, tmp);
}

Monom::Monom(const string& monom){
	match_pattern(monom);
	parse_monom(monom);
}

bool Monom::equal(const Monom& m) const {
	return (degX == m.degX && degY == m.degY && degZ == m.degZ);
}

bool Monom::operator<(const Monom& m)const {
	if (equal(m)) return k < m.k;
	return ((degX * 100 + degY * 10 + degZ) < (m.degX * 100 + m.degY * 10 + m.degZ));
}

bool Monom::operator<=(const Monom& m)const {
	if (equal(m)) return k < m.k;
	return ((degX * 100 + degY * 10 + degZ) <= (m.degX * 100 + m.degY * 10 + m.degZ));
}

bool Monom::operator>(const Monom& m)const {
	return !(*this <= m);
}

bool Monom::operator>=(const Monom& m)const {
	return !(*this < m);
}

bool Monom::operator==(const Monom& m)const {
	return (k == m.k && degX == m.degX && degY == m.degY && degZ == m.degZ);
}

bool Monom::operator!=(const Monom& m)const {
	return !(*this == m);
}

Monom Monom::operator+(const Monom& m)const {
	if (!equal(m)) throw logic_error("monoms arent equal");
	return (Monom {k+m.k,degX, degY, degZ});
}

Monom& Monom::operator+=(const Monom& m) {
	if (!equal(m)) throw logic_error("monoms arent equal");
	k += m.k;
	return (*this);
}

Monom Monom::operator-(const Monom& m) const {
	if (!equal(m)) throw logic_error("monoms arent equal");
	return (Monom{ k - m.k,degX, degY, degZ });
}

Monom& Monom::operator-=(const Monom& m) {
	if (!equal(m)) throw logic_error("monoms arent equal");
	k -= m.k;
	return (*this);
}

Monom Monom::operator*(const Monom& m) const {
	return (Monom{ k * m.k,degX + m.degX, degY + m.degY, degZ + m.degZ });
}

Monom& Monom::operator*=(const Monom& m) {
	k *= m.k;
	degX += m.degX;
	degY += m.degY;
	degZ += m.degZ;
	return (*this);
}

Monom Monom::operator/(const Monom& m) const {
	return (Monom{ k / m.k,degX - m.degX, degY - m.degY, degZ - m.degZ });
}

Monom& Monom::operator/=(const Monom& m) {
	k /= m.k;
	degX -= m.degX;
	degY -= m.degY;
	degZ -= m.degZ;
	return (*this);
}

Monom Monom::operator*(const double& k)const {
	return Monom{this->k*k, degX, degY, degZ};
}

Monom& Monom::operator*=(const double& k) {
	this->k *= k;
	return *this;
}

Monom Monom::operator/(const double& k)const {
	return Monom{ this->k / k, degX, degY, degZ };
}

Monom& Monom::operator/=(const double& k) {
	this->k /= k;
	return *this;
}

ostream& operator<<(ostream& os, const Monom& m) {
	if (m.k == 0) return os;
	else if (m.k == 1) {
		if (m.degX == 0 && m.degY == 0 && m.degZ == 0) {
			os << "1";
			return os;
		}
	}
	else if (m.k == -1) { 
		if (m.degX == 0 && m.degY == 0 && m.degZ == 0) {
			os << "-1";
			return os;
		}
		os << "-"; 
	}
	else os << m.k;
	if (m.degX == 0) os << "";
	else if (m.degX == 1) os << "x";
	else os << "x" << m.degX;
	if (m.degY == 0) os << "";
	else if (m.degY == 1) os << "y";
	else os << "y" << m.degY;
	if (m.degZ == 0) os << "";
	else if (m.degZ == 1) os << "z";
	else os << "z" << m.degZ;
	return os;
}

double Monom::calculate(double x, double y, double z) const {
	return k == 0.0 ? 0.0 : static_cast<double>(k) * powf(x, degX) * powf(y, degY) * powf(z, degZ);
}

double Monom::get_coef() const { return k; }

int Monom::get_x() const { return degX; }

int Monom::get_y() const { return degY; }

int Monom::get_z() const { return degZ; }
