#ifndef __STACK_H__
#define __STACK_H__

#include <stdexcept>

using namespace std;
const int MaxStackSize = 129;

template <class T>
class Stack
{
    T* pMem;                                    // стек
    size_t size;                                // размер стека
    size_t top;                                 // вершина стека
public:
    Stack();                                    // конструктор-инициализатор
    Stack(size_t _size);                        // конструктор-инициализатор
    Stack(const Stack<T>& st);                  // конструктор копирования
    ~Stack();                                   // деструктор
    inline bool empty();                        // нет элементов
    size_t get_size() const noexcept;           // получить размер стека
    T get_top() const;                          // получить верхний элемент
    void push(T val);                           // поместить в стек
    T pop();                                    // извлечь из стека
    //------------------------------------------// операторы
    Stack<T>& operator=(const Stack<T>& st);
    bool operator==(const Stack<T>& st) const;
    bool operator!=(const Stack<T>& st) const;
};

template <class T>
Stack<T>::~Stack() { delete[] pMem; }

template <class T>
Stack<T>::Stack() : Stack(8) {}

template <class T>
Stack<T>::Stack(size_t _size) : top(0), size(_size)
{
    if (size >= MaxStackSize || size <= 0)
        throw invalid_argument("invalid stack size");
    pMem = new T[size]();
}

template <class T>
Stack<T>::Stack(const Stack<T>& st) : top(st.top), size(st.top)
{
    pMem = new T[size]();
    copy(st.pMem, st.pMem + top, pMem);
}

template <class T>
bool Stack<T>::operator==(const Stack<T>& st) const
{
    if (st.pMem == pMem)
        return true;
    if (top != st.top)
        return false;
    for (size_t i = 0; i < top; i++)
        if (pMem[i] != st.pMem[i])
            return false;
    return true;
}

template <class T>
bool Stack<T>::operator!=(const Stack<T>& st) const { return !(*this == st); }

template <class T>
inline bool Stack<T>::empty() { return top == 0; }

template <class T>
size_t Stack<T>::get_size() const noexcept { return top; }

template <class T>
T Stack<T>::get_top() const
{
    if (top <= 0)
        throw runtime_error("stack is empty");
    return(pMem[top - 1]);
}

template <class T>
void Stack<T>::push(T val)
{
    if (top >= size)
    {
        size *= 2;
        if (size > MaxStackSize)
            throw runtime_error("too large stack");
        T* tmp = new T[size];
        copy(pMem, pMem + top, tmp);
        delete[] pMem;
        pMem = tmp;
    }
    pMem[top++] = val;
}

template <class T>
T Stack<T>::pop()
{
    if (top <= 0)
        throw runtime_error("stack is empty");
    return pMem[--top];
}

template <class T>
Stack<T>& Stack<T>::operator=(const Stack<T>& st)
{
    if (pMem == st.pMem)
        return *this;
    else if (top == st.top)
        copy(st.pMem, st.pMem + top, pMem);
    else
    {
        try {
            top = st.top;
            T* tmp = new T[top];
            copy(st.pMem, st.pMem + top, tmp);
            delete[] pMem;
            pMem = tmp;
        }
        catch (...) {
            throw runtime_error("can't create stack");
        }
    }
    return *this;
}
#endif
