#pragma once

// Метафункция IntCons, добавляющая один элемент в голову списка
template<int H, typename IL>
struct IntCons;
template<int H, int... Ints>
struct IntCons<H, IntList<Ints...>>
{
    using type = IntList<H, Ints...>;
};
