#pragma once

// Quantity хранит вещественное число и его размерность в системе СИ
// Требования к Quantity :
//  - Конструктор по умолчанию и explicit конструктор от double.
//  - Метод value(), который возвращает значение типа double.
template<class Dim>
struct Quantity {
private:
    double v;

public:
    Quantity() : v(0) {};
    explicit Quantity(double v) : v(v) {};
    double value() const { return v; }
};
