#include "pch.h"
#include <iostream>

// Функция разбивает последовательность, заданную через итераторы 'p' и 'q' на 'threads' отрезков
// и выполняет вычисления над элементами этих подпоследовательностей в 'threads' потоков.
// Затем выполняется "объединение" результатов вычислений каждого потока.
#define RTYPE decltype(f2(f1(*p), f1(*p)))
template <class It, class Fun1, class Fun2>
auto map_reduce_async(It p, It q, Fun1 f1, Fun2 f2, size_t threads) -> RTYPE
{
    auto f_thread = [](It p, It q, Fun1 f1, Fun2 f2)
    {
        auto res = f1(*p);
        while (++p != q)
            res = f2(res, f1(*p));
        return res;
    };

    std::vector<std::future<RTYPE>> v;
    std::future<RTYPE> fut;
    auto _p = p;
    size_t d       = std::distance(p, q);
    size_t n_base  = d / threads;
    size_t n_extra = d % threads;

    for (int k = 0; p != q; k++)
    {
        if (std::distance(_p, ++p) == (n_base + ( n_extra ? 1 : 0)))
        {
            fut = std::async(std::launch::async, f_thread, _p, p, f1, f2);
            v.push_back(std::move(fut));
            _p = p;
            if (n_extra)
                n_extra--;
        }
    }

    RTYPE res = RTYPE();      // конструктор по умолчанию для типа F2_RTYPE

    for (decltype(fut) & _fut : v)
        res = f2(res, _fut.get());

    return res;
}

// А-ля map_reduce_async, но с использованием std::thread
template <class It, class Fun1, class Fun2>
auto map_reduce_thread(It p, It q, Fun1 f1, Fun2 f2, size_t threads) -> RTYPE
{
    std::vector<std::pair<std::thread, RTYPE>> v;

    auto _p = p;
    
    size_t d = std::distance(p, q);
    size_t n_base = d / threads;
    size_t n_extra = d % threads;

    for (int k = 0; p != q; k++)
    {
        if (std::distance(_p, ++p) == (n_base + (n_extra ? 1 : 0)))
        {
            v.push_back(std::pair<std::thread, RTYPE>());
            RTYPE& r = v.back().second;
            v.back().first = std::thread(
                // лямбда функция, захватывающая 'r' по ссылке;
                // т.е. внутри лямбды будет выполняться работа не над копией 'r', а над той же самой переменной 'r'
                // т.е. изменения переменной внутри люмбды будут "видны" снаружи лямбды
                [&r](It p, It q, Fun1 f1, Fun2 f2)  
                {
                    r = f1(*p);
                    while (++p != q)
                        r = f2(r, f1(*p));
                },
                _p, p, f1, f2
            );

            _p = p;
            if (n_extra)
                n_extra--;
        }
    }

    RTYPE res = RTYPE();            // конструктор по умолчанию для типа F2_RTYPE

    for (std::pair<std::thread, RTYPE>& p : v) {
        p.first.join();             // ждём пока поток закончит выполнение
        res = f2(res, p.second);    // учитываем результат вычислений потока
    }

    return res;
}

template<int N>
struct Fib {
    static int const value = Fib<N-1>::value + Fib<N-2>::value;
};
template<>
struct Fib<1> {
    static int const value = 1;
};
template<>
struct Fib<0> {
    static int const value = 0;
};

int main()
{
    std::list<int> l = { 1,2,3,4,5 };
    // map_reduce_async testing
    {
        // ïàðàëëåëüíîå ñóììèðîâàíèå â 3 ïîòîêà
        auto sum = map_reduce_async(
            l.begin(), l.end(),         // íà÷àëî, êîíåö
            [](int i) {return i; },     // óíàíûé ôóíêòîð
            std::plus<int>(),           // áèíàðíûé ôóíêòîð
            3                           // êîëè÷åñòâî ïîòîêîâ
        );
        // ïðîâåðêà íàëè÷èÿ ÷¸òíûõ ÷èñåë â ÷åòûðå ïîòîêà
        auto has_even = map_reduce_async(
            l.begin(), l.end(),
            [](int i) {return i % 2 == 0; },
            std::logical_or<bool>(),
            4
        );
    }
    // map_reduce_thread testing
    {
        // ïàðàëëåëüíîå ñóììèðîâàíèå â 3 ïîòîêà
        auto sum = map_reduce_thread(
            l.begin(), l.end(),         // íà÷àëî, êîíåö
            [](int i) {return i; },     // óíàíûé ôóíêòîð
            std::plus<int>(),           // áèíàðíûé ôóíêòîð
            3                           // êîëè÷åñòâî ïîòîêîâ
        );
        // ïðîâåðêà íàëè÷èÿ ÷¸òíûõ ÷èñåë â ÷åòûðå ïîòîêà
        auto has_even = map_reduce_thread(
            l.begin(), l.end(),
            [](int i) {return i % 2 == 0; },
            std::logical_or<bool>(),
            4
        );
    }

    std::cout << "Fib<10> = " << Fib<10>::value << std::endl;
}

