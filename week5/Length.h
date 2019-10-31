#pragma once

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
