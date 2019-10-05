#include "pch.h"
#include "bad_from_string.h"

using namespace std;

template<class T>
T from_string(string const& s)
{
    T v;
    istringstream iss(s);
    iss.setf(std::ios_base::skipws);
    iss >> v;
    if (iss.fail() || iss.peek() != EOF)
        throw bad_from_string(s);
    return v;
}
// Safe from string call
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
    //               string  +      +                                                                               
    //               double  +      +                                                                               
    //                  int  +      e                                                                               
    //                 char  +      e                                                                               
    for (auto& str : strings) {
        cout << endl << "from_string(\'" << str << "\'):";
        _SFROM_STRING(string);
        _SFROM_STRING(double);
        _SFROM_STRING(int);
        _SFROM_STRING(char);
        cout << endl;
    }
}

int main()
{
    from_string_test();
}