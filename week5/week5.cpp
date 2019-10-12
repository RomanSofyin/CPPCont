#include "pch.h"
#include <iostream>

template <class It, class Fun1, class Fun2>
auto map_reduce(It p, It q, Fun1 f1, Fun2 f2, size_t num) -> decltype(f1(*p)) {
    // auto res = f1(*p);
    // while (++p != q)
    //     res = f2(res, f1(*p));
    auto res = f1(*p);
    return res;
}

int main()
{
    std::list<int> l = { 1,2,3,4,5,6,7,8,9,10 };
    // ������������ ������������ � 3 ������
    auto sum = map_reduce(
        l.begin(), l.end(),         // ������, �����
        [](int i) {return i; },     // ������ �������
        std::plus<int>(),           // �������� �������
        3                           // ���������� �������
    );
    // �������� ������� ������ ����� � ������ ������
    auto has_even = map_reduce(
        l.begin(), l.end(),
        [](int i) {return i % 2 == 0; },
        std::logical_or<bool>(),
        4
    );
}

