﻿#pragma once

// Dimension представляет размерность вещественного числа в системе СИ
template<int m = 0, int kg = 0, int s = 0, int A = 0, int K = 0, int mol = 0, int cd = 0>
using Dimension = IntList<m, kg, s, A, K, mol, cd>;
