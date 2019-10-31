#pragma once

// Quantity хранит вещественное число и его размерность в системе СИ
// Требования к Quantity :
//  - Конструктор по умолчанию и explicit конструктор от double.
//  - Метод value(), который возвращает значение типа double.
//  - Можно складывать только величины одной размерности.
//  - При умножении(делении) соответствующие размерности поэлементно складываются(вычитаются).
//  - Нужно реализовать умножение и деление на число типа double.
//     + деление double на Quantity
template<template <int, int, int, int, int, int, int> class Dim>
struct Quantity {
private:
    double v;

public:
    Quantity() : v(0) {};
    explicit Quantity(double v) : v(v) {};
    double value() const { return v; }
};
