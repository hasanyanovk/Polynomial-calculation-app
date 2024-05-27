#include "postfix.h"



namespace polish_notation {


    Postfix<double>::Postfix() : infix(""), postfix("") {}

    Postfix<double>::Postfix(string str) : infix(str) {
        split(); validateInfix(eraseSpaces(infix)); to_postfix();
    }

    string Postfix<double>::get_infix() const noexcept { return infix; }

    string Postfix<double>::get_postfix() const noexcept { return postfix; }

    double factorial(size_t n) { return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n; }

    map<string, double> Postfix<double>::get_operands() const  noexcept { return operands; }

    inline void Postfix<double>::update_postfix(const string s) noexcept { postfix += (s == "~" ? "-" : s); }

    bool Postfix<double>::operator==(const Postfix<double>& p) const {
        return (postfix == p.postfix);
    }

    bool Postfix<double>::operator!=(const Postfix<double>& p) const {
        return (postfix != p.postfix);
    }

    Postfix<double>::Postfix(const Postfix& p)
        : infix(p.infix)
        , postfix(p.postfix)
        , operands(p.operands)
        , lexems(p.lexems) {}

    Postfix<double>::Postfix(Postfix&& p) noexcept
        : infix(move(p.infix))
        , postfix(move(p.postfix))
        , operands(move(p.operands))
        , lexems(move(p.lexems)) {}

    map<string, unsigned int> Postfix<double>::priority = {
        {"(", 1}, {"+", 2}, {"-", 2}, {"*", 3}, {"/", 3}, {"%", 3}, {"~", 4}, {"^", 5}, {"!", 5}, {"sin", 6}, {"cos", 6},
        {"exp", 6}, {"tg", 6}, {"abs", 6}, {"sqrt", 6}, {"ln", 6}, {"lg", 6}, {"inc", 6}, {"dec", 6}, {"ceil", 6}, {"floor", 6} };

    map<string, function<double(double, double)> > Postfix<double>::binaryOperations = {
        {"+", [](double a, double b) {return a + b; }},
        {"-", [](double a, double b) {return a - b; }},
        {"/", [](double a, double b) {if (b == 0) throw logic_error("cant be calculated"); return a / b; }},
        {"*", [](double a, double b) {return a * b; }},
        {"%", [](double a, double b) {if (b == 0) throw logic_error("cant be calculated"); return fmod(a,b); }},
        {"^", [](double a, double b) {return pow(a,b); }}
    };

    map<string, function<double(double)>> Postfix<double>::unaryOperations = {
        {"~", [](double a) {return -a; }},
        {"!", [](double a) {if (a < 0 || fmod(a,1.) != 0) throw logic_error("cant be calculated"); return factorial(size_t(a)); }},
        {"sin", [](double a) {return sin(a); }},
        {"cos", [](double a) {return cos(a); }},
        {"exp", [](double a) {return exp(a); }},
        {"tg", [](double a) {if (cos(a) == 0) throw logic_error("cant be calculated"); return sin(a) / cos(a); }},
        {"abs", [](double a) {return a < 0 ? -a : a; }},
        {"sqrt", [](double a) {if (a < 0)  throw logic_error("cant be calculated"); return sqrt(a); }},
        {"ln", [](double a) {if (a <= 0) throw logic_error("cant be calculated"); return log(a); }},
        {"lg", [](double a) {if (a <= 0) throw logic_error("cant be calculated"); return log10(a); }},
        {"inc", [](double a) {return ++a; }},
        {"dec", [](double a) {return --a; }},
        {"ceil", [](double a) {return ceil(a); }},
        {"floor", [](double a) {return floor(a); }}
    };

    inline bool Postfix<double>::is_operator(const string s) {
        const auto& end = Postfix<double>::priority.end();
        return (priority.find(s) != end || s == ")");
    }

    void Postfix<double>::split() {
        if (infix.empty())
            throw invalid_argument("syntax error");
        string elem = "";
        for (const char& cur : infix) {
            if (isalnum(cur) || cur == '.')
                elem += cur;
            else if (is_operator(string{ cur })) {
                if (!elem.empty()) {
                    lexems.push_back(elem);
                    elem = "";
                }
                lexems.push_back(string{ cur });
            }
        }
        if (!elem.empty())
            lexems.push_back(elem);
    }

    void Postfix<double>::to_postfix() {
        Stack<string> stack;
        vector<string> tmp;
        string prev = "";
        for (auto& l : lexems) {
            if (!is_operator(l)) {
                try {
                    operands.emplace(l, stod(l));
                }
                catch (...) {
                    operands.emplace(l, 0);
                }
                tmp.push_back(l);
                if (postfix != "")
                    postfix += " ";
                update_postfix(l);
            }
            else {
                switch (l[0]) {
                case'(':
                    stack.push(l);
                    break;
                case')':
                    while (stack.get_top() != "(") {
                        string t = stack.pop();
                        tmp.push_back(t);
                        update_postfix(t);
                    }
                    stack.pop();
                    break;
                case '-':
                    if (prev == "" || prev == "(")
                        l = "~";
                default:
                    while (!stack.empty() && priority[l] <= priority[stack.get_top()]) {
                        string t = stack.pop();
                        tmp.push_back(t);
                        update_postfix(t);
                    }
                    stack.push(l);
                    break;
                }
            }
            prev = l;
        }
        while (!stack.empty()) {
            string t = stack.pop();
            tmp.push_back(t);
            update_postfix(t);
        }
        lexems = tmp;
    }

    double Postfix<double>::calculate() {
        double a, b;
        Stack<double> stack;
        auto end = operands.end();
        auto _end = binaryOperations.end();
        for (auto& l : lexems) {
            if (operands.find(l) != end)
                stack.push(operands[l]);
            else {
                if (binaryOperations.find(l) != _end) {
                    b = stack.pop(), a = stack.pop();
                    stack.push(binaryOperations[l](a, b));
                }
                else {
                    a = stack.pop();
                    stack.push(unaryOperations[l](a));
                }
            }
        }
        return stack.get_top();
    }

    double Postfix<double>::operator()(map<string, double>& values) {
        double a, b;
        Stack<double> stack;
        auto end = values.end();
        auto _end = binaryOperations.end();
        for (auto& l : lexems) {
            if (values.find(l) != end)
                stack.push(values[l]);
            else {
                if (binaryOperations.find(l) != _end) {
                    b = stack.pop(), a = stack.pop();
                    stack.push(binaryOperations[l](a, b));
                }
                else {
                    a = stack.pop();
                    stack.push(unaryOperations[l](a));
                }
            }
        }
        return stack.get_top();
    }

    ostream& operator<<(ostream& os, const Postfix<double>& p) {
        os << p.get_postfix();
        return os;
    }
}
