#include "pch.h"
#include "bad_from_string.h"

using namespace std;

template<class T>
T from_string(string const& s)
{
    T v;
    istringstream iss(s);
    iss >> noskipws >> v;
    if (iss.fail() || iss.peek() != EOF)
        throw bad_from_string(s);
    return v;
}
// Safe from_string() call
#define _SFROM_STRING(type1)                        \
    try {                                           \
        cout << endl << "<" #type1 ">: ";           \
        cout << from_string<type1>(str);            \
    }                                               \
    catch (exception const& e) {                    \
        cout<<"catch std::exception: "<< e.what();  \
    }                                               \
    catch (...) {                                   \
        cout<<"catch unknown";                      \
    }
void from_string_test()
{    
    vector<string> strings{ "123", "12.3", "", " ", "abc", " 123", "123 ", "12 3", "-1", "a", "a ", " a", "a b", "A" };
    //               string  123    12.3   e    e   "abc"   e       e       e      "-1"  "a"   e     e     e     "A"
    //               double  123    12.3   e    e    e      e       e       e       -1    e    e     e     e      e 
    //                  int  123    e      e    e    e      e       e       e       -1    e    e     e     e      e 
    //                 char  e      e      e   ' '   e      e       e       e       e    'a'   e     e     e     'A' 
    for (auto& str : strings) {
        cout << endl << "from_string(\'" << str << "\'):";
        _SFROM_STRING(string);
        _SFROM_STRING(double);
        _SFROM_STRING(int);
        _SFROM_STRING(char);
        cout << endl;
    }
}

template <class T1, class T2>
struct pair {
    void swap(pair& p) noexcept(
        noexcept(std::swap(first, p.first)) && 
        noexcept(std::swap(second, p.second))
        )
    {
        std::swap(first, p.first);
        std::swap(second, p.second);
    }
    T1 first;
    T2 second;
};

// Обявить функцию do_math как noexcept если:
// - конструктор копирования типа Т является noexcept И
// - оператор присваивания   типа Т является noexcept И
// - оператор сложения       типа Т является noexcept
template<class T>
void do_math() noexcept(
    noexcept(T(declval<T&>())) &&                               // копирование
    noexcept(declval<T&>() = declval<T&>()) &&                  // присваивание
    noexcept(declval<T&>() + declval<T&>())                     // сложение
    )
{
    // we are doing some math here
}

// внутри do_math2 объекты типа T
// - копируются
// - присваиваются
// - перемещаются
// - складываются оператором +
template<class T>
void do_math_() noexcept(
    noexcept(T(std::declval<const T&>())) &&                    // копирование
    noexcept(T(std::declval<T&&>())) &&                         // копирование с перемещением
    noexcept(std::declval<T&>() = std::declval<const T&>()) &&  // присваивание
    noexcept(std::declval<T&>() = std::declval<T&&>()) &&       // присваивание с перемещением
    noexcept(std::declval<T&>() + std::declval<T&>())           // сложение
    )
{
    // тело функции нужно оставить пустым
}

int main()
{
    from_string_test();

    bool b1 = noexcept(do_math<int>()); // true
    bool b2 = noexcept(do_math<std::string>()); // false

    bool b1_ = noexcept(do_math_<int>()); // true
    bool b2 = noexcept(do_math_<std::string>()); // false
}