#include "pch.h"
#include <iostream>

template <class It, class Fun1, class Fun2>
auto map_reduce(It p, It q, Fun1 f1, Fun2 f2, size_t num) -> decltype(f1) {
    // auto res = f1(*p);
    // while (++p != q)
    //     res = f2(res, f1(*p));
    
    auto res = f1(*p);
    
    std::vector<std::future<decltype(f1(*p))>> v;


    for (size_t i = 0; i < num && p != q; i++) {
        //std::future<int> fut = std::async(std::launch::async, map_reduce_mt, );
    }
    
    return res;
    
    // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
    std::function<int(int)> op = [](int x) {return x+1; };
    std::future<int> fut = std::async(std::launch::async, op, 5);
}
template <class It, class Fun1, class Fun2>
auto map_reduce_mt(It p, It q, Fun1 f1, Fun2 f2) -> decltype(f1(*p)) {

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

