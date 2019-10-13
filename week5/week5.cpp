#include "pch.h"
#include <iostream>

#define RTYPE decltype(f2(f1(*p), f1(*p)))
template <class It, class Fun1, class Fun2>
auto map_reduce(It p, It q, Fun1 f1, Fun2 f2, size_t threads) -> RTYPE
{
    auto f_thread = [](It p, It q, Fun1 f1, Fun2 f2)
    {
        auto res = f1(*p);
        while (++p != q)
            res = f2(res, f1(*p));
        return res;
    };

    std::vector<std::future<RTYPE>> v;
    auto _p = p;
    std::future<RTYPE> fut;
    size_t d = std::distance(p, q);
    size_t n = d % threads == 0 ? d / threads : d / threads + 1;

    while (p != q)
    {
        // ��������� �� ���������� �� ��, ��� ������ ������� (std::distance(_p, ++p) == num) ����� ��������� ������?
        // � ������ ���� ������� �����, ��������� �������� ������� ������� ������ ����������� c ������������������ ��������� 'p'.
        if (std::distance(_p, ++p) == n || p == q)
        {
            fut = std::async(std::launch::async, f_thread, _p, p, f1, f2);
            v.push_back(std::move(fut));
            _p = p;
        }
    }

    RTYPE res = RTYPE();      // ����������� �� ��������� ��� ���� RTYPE

    for (decltype(fut) & _fut : v)
    {
        res = f2(res, _fut.get());
    }

    return res;
}


int main()
{
    std::list<int> l = { 1,2,3,4 };
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

