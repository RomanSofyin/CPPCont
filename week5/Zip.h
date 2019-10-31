﻿#pragma once

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
        typename IL2::Tail, Fun
        >::type
    >::type;
};
template<template <int, int> class Fun>
struct Zip<IntList<>, IntList<>, Fun> {
    using type = IntList<>;
};
