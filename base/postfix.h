#ifndef __POSTFIX_H__
#define __POSTFIX_H__

#include "validator.h"
#include "stack.h"
#include "polynom.h"
#include <string>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <regex>
#include <functional>

using namespace std;

namespace polish_notation {

    template<class Type>
    class Postfix{

        static map<string, unsigned int> priority;
        string infix;
        string postfix;
        map<string, Type> operands;
        vector<string> lexems;

        void split();
        void to_postfix();
        static inline bool is_operator(const string& s);
        inline void update_postfix(const string s) noexcept;
        inline bool match_monom(const string& s) const noexcept;
        bool find_char(const string& s) const noexcept;

    public:
        Postfix();
        Postfix(const string& str);
        Postfix(const Postfix& p);
        Postfix(Postfix&& p) noexcept;
        string get_infix() const noexcept;
        string get_postfix() const noexcept;
        const map<string, Type>& get_operands() const;
        Type calculate() const;

        bool operator==(const Postfix<Type>& p) const;
        bool operator!=(const Postfix<Type>& p) const;
        Type operator()(const map<string, Type>& values) const;
        friend ostream& operator<<(ostream& os, const Postfix<Type>& p);
    };

    template<>
    class Postfix<Polynom> {
        static map<string, unsigned int> priority;
        string infix;
        string postfix;
        map<string, Polynom> operands;
        vector<string> lexems;

        void split();
        void to_postfix();
        static inline bool is_operator(const string& s);
        inline void update_postfix(const string s) noexcept;
        inline bool match_monom(const string& s) const noexcept;
        bool find_char(const string& s) const noexcept;

    public:
        Postfix();
        Postfix(const string& str);
        Postfix(const Postfix& p);
        Postfix(Postfix&& p) noexcept;
        string get_infix() const noexcept;
        string get_postfix() const noexcept;
        const map<string, Polynom>& get_operands() const;
        Polynom calculate() const;

        bool operator==(const Postfix<Polynom>& p) const;
        bool operator!=(const Postfix<Polynom>& p) const;
        Polynom operator()(const map<string, Polynom>& values) const;
        friend ostream& operator<<(ostream& os, const Postfix<Polynom>& p);
    };

    template<>
    class Postfix<double> {
        static map<string, function<double(double, double)> > binaryOperations;
        static map<string, function<double(double)>> unaryOperations;
        static map<string, unsigned int> priority;
        string infix;
        string postfix;
        map<string, double> operands;
        vector<string> lexems;
        inline bool is_operator(const string s);
        void split();
    public:
        void to_postfix();
        inline void update_postfix(const string s) noexcept;
    public:
        Postfix();
        Postfix(string str);
        Postfix(const Postfix& p);
        Postfix(Postfix&& p) noexcept;
        string get_infix() const noexcept;
        string get_postfix() const noexcept;
        map<string, double> get_operands() const noexcept;
        double calculate();

        bool operator==(const Postfix<double>& p) const;
        bool operator!=(const Postfix<double>& p) const;
        double operator()(map<string, double>& values);
        friend ostream& operator<<(ostream& os, const Postfix<double>& p);
    };
}
#endif