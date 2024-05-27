#include <iostream>
#include "postfix.h"
#include "database.h"
#include <windows.h>
#include <fstream>

enum Mode {
    Polynom_calculator = 1,
    Double_calculator = 0
};

Database<string, Polynom> database;
bool p_first_time = true;
bool d_first_time = true;

int main() {
    HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_GREEN | 0);
    setlocale(LC_ALL, "ru");
    setlocale(LC_NUMERIC, "en"); 
    Mode mode = Mode::Polynom_calculator;
start:
    if(mode) {
        system("title ����������� ���������");
        string expression, operation;
        auto& binary_op = Polynom::binary_operations;
        auto& unary_op = Polynom::unary_operations;
        auto& ternary_op = Polynom::ternary_operations;
        auto& integrals_op = Polynom::integrals;
        bool flag = 0;     
        ifstream file("helper_polynom.txt");
        static char buff[1024][1024];
        int i = 0;
        system("cls");
        if (p_first_time) {
            while (!file.eof()) {
                file.getline(buff[i], sizeof(buff));
                cout << buff[i] << endl;
                i++;
            }
            p_first_time = false;
        }
        file.close();
        while (!flag)
        {
            SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_GREEN | 0);
            cout << "������� �������������� ���������: ";
            getline(cin, expression);
            try {
                Polynom tmp(expression);
                flag = true;
            }
            catch (invalid_argument) {
                SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED | 0);
                cout << "�������� ���������" << endl;
                system("PAUSE");
                system("cls");
            }
        }
        Polynom p(expression);
    label:
        while (1) {
            SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_GREEN | 0);
            system("cls");
            cout << "������� �������������� ���������: ";
            cout << p << endl;
            cout << "������� ��������: ";
            getline(cin, operation);
            operation = eraseSpaces(operation);
            if (binary_op.find(operation) != binary_op.end()) {
                string exp;
                cout << "������� ������ ���������: ";
                getline(cin, exp);
                flag = 0;
                try {
                    Polynom tmp(exp);
                    flag = true;
                }
                catch (invalid_argument) {
                    SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED | 0);
                    cout << "�������� ���������" << endl;
                    system("PAUSE");
                    goto label;
                }
                Polynom p1(exp);
                p = binary_op[operation](p, p1);
            }
            else if (unary_op.find(operation) != unary_op.end())
                p = unary_op[operation](p);
            else if (ternary_op.find(operation) != ternary_op.end()) {
                Point point;
                cout << "������� ���������� �����(x,y,z): ";
                cin >> point;
                p = Polynom{ to_string(ternary_op[operation](p,point)) };
            }
            else if (integrals_op.find(operation) != integrals_op.end()) {
                Point point1, point2;
                cout << "������� ���������� ������ �����(x,y,z): ";
                cin >> point1;
                cout << "������� ���������� ������ �����(x,y,z): ";
                cin >> point2;
                p = Polynom{ to_string(integrals_op[operation](p, point1, point2)) };
            }
            else if (operation == "save") {
                string name, pol;
                system("cls");
                cout << "������� ��� ��������: ";
                getline(cin, name);
                cout << "������� ��������: ";
                getline(cin, pol);
                try {
                    Polynom p(pol);
                    database.emplace(name, p);
                }
                catch (runtime_error) {
                    SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED | 0);
                    cout << "��� ��� ������������" << endl;
                    system("PAUSE");
                    goto label;
                }
                catch (...) {
                    SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED | 0);
                    cout << "�������� ���������" << endl;
                    system("PAUSE");
                    goto label;
                }
            }
            else if (operation == "saveas") {
                string name;
                cout << "������� ���: ";
                getline(cin, name);
                try {
                    database.emplace(name, p);
                }
                catch (runtime_error) {
                    SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED | 0);
                    cout << "��� ��� ������������" << endl;
                    system("PAUSE");
                    goto label;
                }
                catch (...) {
                    SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED | 0);
                    cout << "�������� ���������" << endl;
                    system("PAUSE");
                    goto label;
                }
            }
            else if (operation == "download") {
                string name;
                cout << "������� ��� �����: ";
                getline(cin, name);
                try {
                    ofstream fout(string{ name + ".txt" });
                    fout << database;
                    fout.close();
                }
                catch (...) {
                    SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED | 0);
                    cout << "������ �������� �����" << endl;
                    system("PAUSE");
                    goto label;
                }
            }
            else if (operation == "expression" || operation == "expr") {
                string str;
                system("cls");
                cout << "������� ���������: ";
                getline(cin, str);
                try {
                    polish_notation::Postfix<Polynom> postfix(str);
                    auto map = postfix.get_operands();
                    for (auto& a : map) {
                        if (a.second.empty()) {
                            if (database.include(a.first))
                                a.second = database[a.first];
                            else {
                                string s;
                                bool flag2 = true;
                                cout << "������� �������� ��� " << a.first << ": ";
                                while (flag2) {
                                    getline(cin, s);
                                    try {
                                        a.second = Polynom{ s };
                                        flag2 = false;
                                    }
                                    catch (...) {
                                        cout << "������" << endl;
                                        system("PAUSE");
                                        goto label;
                                    }
                                }
                            }
                        }
                    }
                    cout << "�������������� ���������: " << postfix.get_infix() << endl;
                    cout << "����������� �����: " << postfix.get_postfix() << endl;
                    p = postfix(map);
                    cout << "������������ ��������: " << p << endl;
                    system("PAUSE");
                }
                catch (...) {
                    SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED | 0);
                    cout << "�������� ���������" << endl;
                    system("PAUSE");
                    goto label;
                }

            }
            else if (operation == "delete") {
                string name;
                system("cls");
                cout << "������� ���: ";
                try {
                    getline(cin, name);
                }
                catch (...) {
                    cout << "������" << endl;
                    system("PAUSE");
                    goto label;
                }
                database.remove(name);
            }
            else if (operation == "setactive" || operation == "set") {
                string name;
                system("cls");
                cout << "������� �������� �������: ";
                try {
                    getline(cin, name);
                    database.set_active(name);
                }
                catch (...) {
                    cout << "������" << endl;
                    system("PAUSE");
                    goto label;
                }
            }
            else if (operation == "show" || operation == "showdatabase") {
                system("cls");
                cout << database;
                system("PAUSE");
            }
            else if (operation == "help") {
                file.open("helper_polynom.txt");
                while (!file.is_open()) {
                    cerr << "File error!" << endl;
                    std::cin.ignore();
                    goto label;
                }
                int i = 0;
                system("cls");
                while (!file.eof()) {
                    file.getline(buff[i], sizeof(buff));
                    cout << buff[i] << endl;
                    i++;
                }
                file.close();
                system("PAUSE");
            }
            else if (operation == "change" || operation == "changemode") {
                mode = Mode::Double_calculator;
                goto start;
            }
            else if (operation == "exit")
                exit(0);
            else if (operation == "clr" || operation == "clear")
                p = Polynom{};
        }
    }
    else {
        system("title ����������� �������� �������� ������");
        ifstream file("helper_double.txt");
        static char buff[1024][1024];
        int i = 0;
        system("cls");
        if (d_first_time) {
            while (!file.eof()) {
                file.getline(buff[i], sizeof(buff));
                cout << buff[i] << endl;
                i++;
            }
            d_first_time = false;
            system("PAUSE");
        }
        while(1) {
            system("cls");
            SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_GREEN | 0);
            std::cout << "������� �������������� ���������: ";
            string expression = "";
            while(expression == "")
                getline(cin, expression);
            expression = eraseSpaces(expression);
            if (expression == "exit")
                exit(0);
            else if (expression == "change" || expression == "changemode") {
                mode = Mode::Polynom_calculator;
                goto start;
            }
            else if (expression == "show" || expression == "showdatabase") {
                system("cls");
                cout << database;
                system("PAUSE");
            }
            else if (expression == "download") {
                string name;
                cout << "������� ��� �����: ";
                getline(cin, name);
                try {
                    ofstream fout(string{ name + ".txt" });
                    fout << database;
                    fout.close();
                }
                catch (...) {
                    SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED | 0);
                    cout << "������ �������� �����" << endl;
                    system("PAUSE");
                    goto start;
                }
            }
            else if (expression == "help") {
                file.open("helper_double.txt");
                while (!file.is_open()) {
                    cerr << "File error!" << endl;
                    std::cin.ignore();
                    goto start;
                }
                int i = 0;
                system("cls");
                while (!file.eof()) {
                    file.getline(buff[i], sizeof(buff));
                    cout << buff[i] << endl;
                    i++;
                }
                file.close();
                system("PAUSE");
            }
            try {
                polish_notation::Postfix<double> postfix(expression);
                auto m = postfix.get_operands();
                for (auto& op : m) {
                    if (isalpha(op.first[0])) {
                        string val;
                        std::cout << "������� �������� ��� " << op.first << ": ";
                        cin >> val;
                        op.second = stod(val);
                    }
                }
                std::cout << "�������������� ���������: " << postfix.get_infix() << endl;
                std::cout << "����������� �����: " << postfix.get_postfix() << endl;
                std::cout << "������������ ��������: " << postfix(m) << endl;
                system("PAUSE");
            }
            catch (invalid_argument) {
                SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED | 0);
                std::cout << "�������� ���������" << endl;
                system("PAUSE");
            }
            catch (logic_error) {
                SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_RED | 0);
                std::cout << "������ ����������" << endl;
                system("PAUSE");
            }
        }
    }
    return 0;
}
