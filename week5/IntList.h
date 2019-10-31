#pragma once

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
