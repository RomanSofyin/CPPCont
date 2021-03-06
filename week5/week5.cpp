﻿#include "pch.h"
#include <iostream>

#include "Fib.h"
#include "IntList.h"
#include "Length.h"
#include "IntCons.h"
#include "Generate.h"
#include "Zip.h"
#include "MetaArith.h"
#include "Quantity.h"
#include "Dimension.h"
#include "SizeStruct.h"

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

/* * * * * * * * * * * * * * * * * * * * * *
 * Вывод списка IntList разными способами  *
 * * * * * * * * * * * * * * * * * * * * * */
template<typename T> void print_tmpl_parm()  { std::cout << __FUNCSIG__ << std::endl; }
template<typename T> void print_tmpl_par2()  { std::cout << typeid(T).name() << "\n"; }
template<typename IL> void print_tmpl_par3() { std::cout << IL::Head << " "; print_tmpl_par3< IL::Tail>(); }
template<> void print_tmpl_par3<IntList<>>() { std::cout << std::endl; }

// Воспользоваться IntList и метафункцией Generate для того, чтобы научиться "раскрывать" кортежи.
// Требуется написать функцию apply, которая принимает функтор и кортеж с аргументами
// для вызова этого функтора и вызывает функтор от этих аргументов.
// Хорошее описание здесь - https://blog.galowicz.de/2016/06/24/integer_sequences_at_compile_time/
template <
    typename F,
    typename Tuple, 
    int ... indxs>  // в шаблоне ожидается не одно значение типа int, а список значений типа int; например "0,1,2"
auto apply_(F f, Tuple t, IntList<indxs...>) -> decltype(f(std::get<indxs>(t)...))
{
    // здесь, при вызове функции 'f', проиходит вызов шаблонного метода std::get для кортежа 't', с указанием кажого из значений "indxs"
    // т.е. если в качестве indxs шаблона IntList (переданного третьим параметром) переданы "0,1,2", то получется конструкция следующего вида:
    // f(std::get<0>(t), std::get<1>(t), std::get<2>(t))
    return f(std::get<indxs>(t)...);
}
template <
    typename F, 
    typename ... Args>  // в шаблоне ожидается использование конструкций, которые принимают не один тип, а список типов
auto apply(F f, 
    const std::tuple<Args...>& t) // функция "apply" принимает кортеж не от одного шаблонного аргумента, а от списка шаблонных аргументов, которые являются типами
    -> decltype(apply_(f, t, typename Generate<sizeof...(Args)>::type{}))
{
    // "sizeof...(Args)" позволяет на этапе компиляции вычислить количество переданных шаблонных параметров
    using intList = typename Generate<sizeof...(Args)>::type;
    std::cout << typeid(intList).name() << "\n";
    return apply_(f, t,
        intList{}); // здесь в функцию передаётся экземпляр типа "intList"
}

// Операторы для класса Quantity:
//  - Можно складывать только величины одной размерности.
//  - При умножении/делении соответствующие размерности поэлементно складываются/вычитаются.
//  - Нужно реализовать умножение и деление на число типа double.
//     + деление double на Quantity
// на помощь - https://benjaminjurke.com/content/articles/2015/compile-time-numerical-unit-dimension-checking/
template <typename IL>
auto operator+(const Quantity<IL>& lhs, const Quantity<IL>& rhs) -> decltype(Quantity<IL>())
{
    double newValue = lhs.value() + rhs.value();
    return Quantity<IL>(newValue);
}

template <typename IL>
auto operator-(const Quantity<IL>& lhs, const Quantity<IL>& rhs) -> decltype(Quantity<IL>())
{
    double newValue = lhs.value() - rhs.value();
    return Quantity<IL>(newValue);
}

template <typename IL1, typename IL2>
auto operator*(const Quantity<IL1>& lhs, const Quantity<IL2>& rhs) -> decltype(Quantity<typename Zip<IL1, IL2, Plus>::type>())
{
    double newValue = lhs.value() * rhs.value();
    return Quantity<typename Zip<IL1, IL2, Plus>::type>(newValue);
}
template <typename IL1>
auto operator*(const Quantity<IL1>& lhs, const double r) -> decltype(Quantity<IL1>())
{
    double newValue = lhs.value() * r;
    return Quantity<IL1>(newValue);
}
template <typename IL2>
auto operator*(const double l, const Quantity<IL2>& rhs) -> decltype(Quantity<IL2>())
{
    double newValue = l * rhs.value();
    return Quantity<IL2>(newValue);
}

template <typename IL1, typename IL2>
auto operator/(const Quantity<IL1>& lhs, const Quantity<IL2>& rhs) -> decltype(Quantity<typename Zip<IL1, IL2, Minus>::type>())
{
    double newValue = lhs.value() / rhs.value();
    return Quantity<typename Zip<IL1, IL2, Minus>::type>(newValue);
}
template <typename IL1>
auto operator/(const Quantity<IL1>& lhs, const double r) -> decltype(Quantity<IL1>())
{
    double newValue = 0;
    if (r)
        newValue = lhs.value() / r;
    return Quantity<IL1>(newValue);
}
template <typename IL2>
auto operator/(const double l, const Quantity<IL2>& rhs) -> decltype(Quantity<typename UnaryZip<IL2, Neg>::type>())
{
    double newValue = l / rhs.value();
    return Quantity<typename UnaryZip<IL2, Neg>::type>(newValue);
}

template<typename T>
struct has_size {
private:
    // Версия метода detect, которая будет существовать для любого типа Т, переданного в has_size
    static void detect(...);
    // Версия метода detect, которая будет существовать только для типа Т, содержащего функцию "size" без параметров - "size()"
    // Это достигается за счёт использования оператора decltype, который  определяет тип выражения, причём само выражение не вычисляется и в код не переводится.
    // В качестве выражения для decltype используется вызов требуемого метода, с аргументами нужного типа. Тогда результатом decltype будет возвращаемый тип метода.
    // Если метода с таким именем нет, или он принимает другие типы аргументов, то получится требуемая ошибка, которая позволит реализовать механизм SFINAE.
    // ---
    // Чтобы не требовать от типа U наличия конструктора по умолчанию, используется declval.
    template<typename U> static decltype(std::declval<U>().size()) detect(const U&);
public:
    // Сравнивается тип size_t с типом, который возвращается одной из версий detect().
    // Если при подстановке типа Т в метод detect произошёл выбор 2ой версии этого метода, то тип возвращаемого значения будет size_t и "is_same" вернёт true.
    static constexpr bool value = std::is_same<size_t, decltype(detect(std::declval<T>()))>::value;
};

// Шаблонная функция get_size, которая принимает значение некоторого типа,
// у которого есть либо константный метод size() возвращающий size_t, либо поле size типа size_t,
// и возвращает соответствующее значение.
template <typename T, class E = typename std::enable_if<has_size<T>::value, T>::type>
auto get_size(const T& t) -> decltype(t.size()) {
    return t.size();
}
template <typename T, class E = typename std::enable_if<!has_size<T>::value, T>::type>
auto get_size(const T& t) -> decltype(t.size) {
    return t.size;
}


int main()
{
    // map_reduce_*
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
    }
    // Fib
    {
        std::cout << "Fib<10> = " << Fib<10>::value << std::endl;
    }
    //IntList
    {
        using primes = IntList<2, 3, 5, 7, 11, 13>;
        constexpr int head = primes::Head;
        using odd_primes = primes::Tail;
    }
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
        auto res = apply(f, t);                 // res = 36.6
    }
    // Zip
    {
        // два списка одной длины
        using L1 = IntList<1, 2, 3, 4, 5>;
        using L2 = IntList<1, 3, 7, 7, 2>;
        // результат применения — список с поэлементными суммами
        using L3 = Zip<L1, L2, Plus>::type;  // IntList<2, 5, 10, 11, 7>
        print_tmpl_par3<L3>();
    }
    // Quantity, Dimension
    {        
        LengthQ   l{ 30000 };           // 30 км
        TimeQ     t{ 10 * 60 };         // 10 минут
        // ll = 3*l
        LengthQ lll1 = l + l + l;       // 90 км
        LengthQ lll2 = 3 * l;           // 90 км
        LengthQ lll3 = l * 3;           // 90 км
        LengthQ ll = lll1 - (2 * l);    // 30 км
        
        // вычисление скорости
        VelocityQ v = l / t;            // результат типа VelocityQ, 50 м/с
        
        AccelQ    a{ 9.8 };             // ускорение свободного падения
        MassQ     m{ 80 };              // 80 кг
        // f - сила притяжения, которая действует на тело массой 80 кг
        ForceQ    F1 = m * a;           // Сила притяжения = 784 (кг*м/с^2 = Ньютон)
        ForceQ    F2 = F1 / 2;          // f пополам = 392       (кг*м/с^2 = Ньютон)

        FreqQ     f{ 25 };              // Частота = 25          (1/с = Герц)
        TimeQ     T = 1 / f;            // Период = 0.04 с
    }
    // SFINAE practice
    {
        std::string s{ "Hello" };
        size_t s_size = get_size(s);   // 5, вызывается метод size()

        SizeStruct x{ 10 };
        size_t x_size = get_size(x);  // 10, читается поле size
    }
}

