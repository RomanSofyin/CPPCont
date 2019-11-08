#pragma once

// ToDo: написать вариант метафункции Zip, которая бы работала с произвольным числом
// списков IntList и шаблонной функцией (Fun), принимающей произвольное число шаблонных аргементов

// Zip - (аналог std::transform) принимает два списка целых чисел одинаковой длины,
// а так же бинарную метафункцию, и возвращает список, получившийся в результате
// поэлементного применения метафункции к соответствующим элементам исходных списков.
template<
    typename IL1,
    typename IL2,
    template <int, int> class Fun   // шаблонный параметр, который является шаблоном (шаблонным классом - "class Fun") с двумя целочисленными шаблонными параметрами
>
struct Zip;
template<typename IL1, typename IL2, template <int, int> class Fun>
struct Zip {
    using type = typename IntCons<
        Fun<IL1::Head, IL2::Head>::value,
        typename Zip<
            typename IL1::Tail,
            typename IL2::Tail,
            Fun
        >::type
    >::type;
};
template<template <int, int> class Fun>
struct Zip<IntList<>, IntList<>, Fun> { // специализация шаблона Zip для случая, когда он был вызван с двумя пустыми IntList и некой бинарной функцией Fun
    using type = IntList<>;
};

// UnaryZip - принимает список целых чисел а также унарную метафункцию, и возвращает список,
// получившийся в результате поэлементного применения метафункции к элементам исходного списка.
template<
    typename IL1,
    template <int> class Fun   // шаблонный параметр, который является шаблоном (шаблонным классом - "class Fun") с одним целочисленным шаблонным параметром
>
struct UnaryZip;
template<typename IL1, template <int> class Fun>
struct UnaryZip {
    using type = typename IntCons<
        Fun<IL1::Head>::value,
        typename UnaryZip<
            typename IL1::Tail,
            Fun
        >::type
    >::type;
};
template<template <int> class Fun>
struct UnaryZip<IntList<>, Fun> {
    using type = IntList<>;
};