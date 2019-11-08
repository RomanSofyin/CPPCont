#pragma once

// бинарная метафункция Plus
template<int a, int b>
struct Plus
{
    static int const value = a + b;
};

// бинарная метафункция Minus
template<int a, int b>
struct Minus
{
    static int const value = a - b;
};

// бинарная метафункция Neg
template<int a>
struct Neg
{
    static int const value = -(a);
};