#pragma once

// Dimension представляет размерность вещественного числа в системе СИ.
//
// Dimension - это такой IntList, у которого длина 7 и у которого все шаблонные параметры
// (являющиеся по сути элементами этого IntList) по умолчанию равны 0.
template<int m = 0, int kg = 0, int s = 0, int A = 0, int K = 0, int mol = 0, int cd = 0>
using Dimension = IntList<m, kg, s, A, K, mol, cd>;

using NumberQ   = Quantity<Dimension<>>;            // число без размерности
using LengthQ   = Quantity<Dimension<1>>;           // метры
using MassQ     = Quantity<Dimension<0, 1>>;        // килограммы
using TimeQ     = Quantity<Dimension<0, 0, 1>>;     // секунды
using FreqQ     = Quantity<Dimension<0, 0, -1>>;    // частота
using VelocityQ = Quantity<Dimension<1, 0, -1>>;    // метры в секунду
using AccelQ    = Quantity<Dimension<1, 0, -2>>;    // ускорение, метры в секунду в квадрате
using ForceQ    = Quantity<Dimension<1, 1, -2>>;    // сила в ньютонах
