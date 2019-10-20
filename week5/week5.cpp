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

/* * * * * * * * * *
 * Шаблон IntList  *
 * * * * * * * * * */
// Определяем список целых чисел
template <int ... Ints>
struct IntList;
// специализация по умолчанию - в списке есть минимум 1 элемент (голова), хвост может быть пустой
template <int H, int ... Ints>
struct IntList<H, Ints...>
{
    static const int Head = H;
    using Tail = IntList<Ints...>;
};
// специализация для пустого списка
template <>
struct IntList<> { };

// Метафункция Length для вычисления длины списка IntList
template<typename IL>
struct Length
{
    static int const value = 1 +
        Length<typename IL::Tail>::value;   // "typename" необходим, чтобы сослаться к члену шаблонного параметра, который является типом
};
template<>
struct Length<IntList<>>
{
    static int const value = 0;
};

// Метафункция IntCons, добавляющая один элемент в голову списка
template<int H, typename IL>
struct IntCons;
template<int H, int... Ints>
struct IntCons<H, IntList<Ints...>>
{
    using type = IntList<H, Ints...>;
};

// Метафункция Generate, создающая шаблон IntList, содержащий указанное число элементов
template<int N, int K = 0>
struct Generate;
template<int N, int K>
struct Generate
{
    using type = typename IntCons<K, typename Generate<N-1, K+1>::type>::type;
};
template<int N>
struct Generate<0, N>
{
    using type = IntList<>;
};
/*
   Возможно такое определение Generate c пар-ом по умолчанию тоже можно использовать:
template<int N, typename IL = IntList<>>
struct Generate;
*/

/* * * * * * * * * * * * * * * * * * * * * *
 * Вывод списка IntList разными способами  *
 * * * * * * * * * * * * * * * * * * * * * */
template<typename T>
void print_tmpl_parm()  { std::cout << __FUNCSIG__ << std::endl; }
template<typename T>
void print_tmpl_par2() { std::cout << typeid(T).name() << "\n"; }
template<typename IL>
void print_tmpl_par3() {
    std::cout << IL::Head << " ";
    print_tmpl_par3< IL::Tail>();
};
template<>
void print_tmpl_par3<IntList<>>()
{
    std::cout << std::endl;
};

// Воспользоваться IntList и метафункцией Generate для того, чтобы научиться "раскрывать" кортежи.
// Требуется написать функцию apply, которая принимает функтор и кортеж с аргументами
// для вызова этого функтора и вызывает функтор от этих аргументов.
template <typename F, typename ... Args>
/*auto*/ void apply(F f, std::tuple<Args...> & t) /*-> decltype(f)*/ {
    /*int a = 5;
    a++;*/
}

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

    using primes = IntList<2, 3, 5, 7, 11, 13>;
    constexpr int head = primes::Head;
    using odd_primes = primes::Tail;

    // Length
    {
        using primes = IntList<2, 3, 5, 7, 11, 13>;
        constexpr size_t len = Length<primes>::value; // 6
    }
    // IntCons, Generate, print_tmpl_par*
    {
        using L1 = IntList<2, 3, 4>;
        using L2 = IntCons<1, L1>::type;   // IntList<1,2,3,4>
        using L3 = Generate<5>::type;      // IntList<0,1,2,3,4>
        print_tmpl_parm<L3>();
        print_tmpl_par2<L3>();
        print_tmpl_par3<L3>();
    }
    // apply()
    {
        auto f = [](int x, double y, double z) { return x + y + z; };
        auto t = std::make_tuple(30, 5.0, 1.6); // std::tuple<int, double, double>
        /*auto res = apply(f, t);*/                 // res = 36.6
        apply(f, t);                 // test
    }
}