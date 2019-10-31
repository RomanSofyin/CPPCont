#pragma once

// бинарная метафункция Plus
template<int a, int b>
struct Plus
{
    static int const value = a + b;
};
