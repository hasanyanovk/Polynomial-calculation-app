#include "postfix.h"

namespace polish_notation {

  
    ostream& operator<<(ostream& os, const Postfix<Polynom>& p) {
        os << p.get_postfix();
        return os;
    }

    map<string, unsigned int> Postfix<Polynom>::priority = { {"(", 1}, {"+", 2}, {"-", 2}, {"*", 3}, {"/", 3}, {"~", 4},
                                                             {"dx", 5}, {"dy", 5}, {"dz", 5}, {"Fdx", 5}, {"Fdy", 5}, {"Fdz", 5} };

    string Postfix<Polynom>::get_infix() const noexcept { return infix; }

    string Postfix<Polynom>::get_postfix() const noexcept { return postfix; }

    const map<string, Polynom>& Postfix<Polynom>::get_operands() const { return operands; }

    bool Postfix<Polynom>::operator==(const Postfix<Polynom>& p) const {
        return (postfix == p.postfix);
    }

    bool Postfix<Polynom>::operator!=(const Postfix<Polynom>& p) const {
        return (postfix != p.postfix);
    }

    void Postfix<Polynom>::split() {
        if (infix.empty())
            throw invalid_argument("syntax error");
        string elem = "";
        for (const char& cur : infix)
        {
            if (isalnum(cur) || cur == '.')
                elem += cur;
            else if (is_operator(string{ cur }))
            {
                if (!elem.empty())
                {
                    lexems.push_back(elem);
                    elem = "";
                }
                lexems.push_back(string{ cur });
            }
        }
        if (!elem.empty())
            lexems.push_back(elem);
    }

    void Postfix<Polynom>::to_postfix() {
        Stack<string> stack;
        vector<string> tmp;
        string prev = "";
        for (auto& l : lexems) {
            if (!is_operator(l)) {
                try {
                    operands.emplace(l, Polynom{ l });
                }
                catch (...) {
                    operands.emplace(l, Polynom{});
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

    inline bool Postfix<Polynom>::is_operator(const string& s) { return (Postfix<Polynom>::priority.find(s) != Postfix<Polynom>::priority.end() || s == ")"); }

    Postfix<Polynom>::Postfix() :infix(""), postfix("") {}

    Postfix<Polynom>::Postfix(const string& str) : infix(str) {
        split();
        validateInfix(eraseSpaces(infix));
        to_postfix();
    }

    Postfix<Polynom>::Postfix(const Postfix& p)
        : infix(p.infix)
        , postfix(p.postfix)
        , operands(p.operands)
        , lexems(p.lexems) {}

    Postfix<Polynom>::Postfix(Postfix&& p) noexcept
        : infix(move(p.infix))
        , postfix(move(p.postfix))
        , operands(move(p.operands))
        , lexems(move(p.lexems)) {}

    bool Postfix<Polynom>::match_monom(const string& s) const noexcept {
        regex pattern("([+-]?[\\d.]*)(x\\d*?)?(y\\d*?)?(z\\d*?)?");
        if (regex_match(s, pattern) && find_char(s))
            return true;
        return false;
    }

    bool Postfix<Polynom>::find_char(const string& s) const noexcept {
        for (const auto& c : s) {
            if (c == 'x' || c == 'y' || c == 'z')
                return true;
        }
        return false;
    }

    void Postfix<Polynom>::update_postfix(const string s) noexcept { postfix += (s == "~" ? "-" : s); }

    Polynom Postfix<Polynom>::operator() (const map<string, Polynom>& values) const {
        Polynom a, b;
        Stack<Polynom> stack;
        auto end = values.end();
        auto binary_end = Polynom::binary_operations.end();
        for (auto& l : lexems) {
            if (values.find(l) != end)
                stack.push((values.find(l))->second);
            else {
                if (Polynom::binary_operations.find(l) != binary_end) {
                    b = stack.pop(), a = stack.pop();
                    stack.push(Polynom::binary_operations[l](a, b));
                }
                else {
                    a = stack.pop();
                    stack.push(Polynom::unary_operations[l](a));
                }
            }
        }
        return stack.get_top();
    }

    Polynom Postfix<Polynom>::calculate() const { return this->operator()(operands); }
}