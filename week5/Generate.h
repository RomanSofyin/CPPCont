#pragma once

// Метафункция Generate, создающая шаблон IntList, содержащий указанное число элементов
template<int N, int K = 0>
struct Generate;
template<int N, int K>
struct Generate
{
    using type = typename IntCons<K, typename Generate<N - 1, K + 1>::type>::type;
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
