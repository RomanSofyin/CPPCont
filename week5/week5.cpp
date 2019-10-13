#include "pch.h"
#include <iostream>

#define F1_RTYPE decltype(f1(*p))
#define F2_RTYPE decltype(f2(f1(*p), f1(*p)))
template <class It, class Fun1, class Fun2>
auto map_reduce(It p, It q, Fun1 f1, Fun2 f2, size_t num) -> F2_RTYPE
{
    auto thread = [](It p, It q, Fun1 f1, Fun2 f2)
    {
        auto res = f1(*p);
        while (++p != q)
            res = f2(res, f1(*p));
        return res;
    };

    std::vector<std::future<F2_RTYPE>> v;
    auto _p = p;
    std::future<F2_RTYPE> fut;

    while (p != q)
    {
        if (std::distance(_p, ++p) == num)
        {
            fut = std::async(std::launch::async, thread, _p, p, f1, f2);
            v.push_back(std::move(fut));
            _p = p;
        }
    }
    if (_p != p)
    {
        fut = std::async(std::launch::async, thread, _p, p, f1, f2);
        v.push_back(std::move(fut));
    }

    F2_RTYPE res = F2_RTYPE();

    for (decltype(fut) & _fut : v)
    {
        res = _fut.get();
    }

    return res;
}


int main()
{
    std::list<int> l = { 1,2,3,4,5,6,7,8,9,10 };
    // параллельное суммирование в 3 потока
    auto sum = map_reduce(
        l.begin(), l.end(),         // начало, конец
        [](int i) {return i; },     // унаный функтор
        std::plus<int>(),           // бинарный функтор
        3                           // количество потоков
    );
    // проверка наличия чётных чисел в четыре потока
    auto has_even = map_reduce(
        l.begin(), l.end(),
        [](int i) {return i % 2 == 0; },
        std::logical_or<bool>(),
        4
    );
}

