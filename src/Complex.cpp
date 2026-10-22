#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include "Complex.h"

int Complex::objectCount = 0;

/**
 * @brief Копирует данные в существующее число
 */
Complex& Complex::operator=(const Complex& other) = default;

/**
 * @brief Создает комплексное число с нулевыми частями
 */
Complex::Complex()
    : Complex(0.0, 0.0)
{
}

/**
 * @brief Создает комплексное число с заданными частями
 */
Complex::Complex(double realPart, double imaginaryPart)
    : realPart(realPart), imaginaryPart(imaginaryPart)
{
    if (!std::isfinite(realPart) || !std::isfinite(imaginaryPart) ||
        !std::isfinite(std::hypot(realPart, imaginaryPart)))
    {
        throw std::invalid_argument("Части числа и его модуль должны быть конечными");
    }

    ++objectCount;
}

/**
 * @brief Создает копию комплексного числа
 *
 * @param other Число для копирования
 */
Complex::Complex(const Complex& other)
    : realPart(other.realPart), imaginaryPart(other.imaginaryPart)
{
    ++objectCount;
}

/**
 * @brief Завершает время жизни числа и уменьшает счетчик
 */
Complex::~Complex()
{
    --objectCount;
}

/**
 * @brief Изменяет действительную часть, проверяя результат
 */
void Complex::changeRealPart(double change)
{
    double newRealPart = realPart + change;

    if (!std::isfinite(newRealPart) || newRealPart == realPart ||
        !std::isfinite(std::hypot(newRealPart, imaginaryPart)))
    {
        throw std::overflow_error("Изменение действительной части нельзя сохранить");
    }

    realPart = newRealPart;
}

/**
 * @brief Увеличивает действительную часть на один
 */
Complex& Complex::operator++()
{
    changeRealPart(1.0);
    return *this;
}

/**
 * @brief Увеличивает действительную часть и возвращает старое число
 */
Complex Complex::operator++(int)
{
    Complex oldValue(*this);
    ++(*this);
    return oldValue;
}

/**
 * @brief Уменьшает действительную часть на один
 */
Complex& Complex::operator--()
{
    changeRealPart(-1.0);
    return *this;
}

/**
 * @brief Уменьшает действительную часть и возвращает старое число
 */
Complex Complex::operator--(int)
{
    Complex oldValue(*this);
    --(*this);
    return oldValue;
}

/**
 * @brief Проверяет и сохраняет результат арифметической операции
 */
void Complex::assignResult(long double newRealPart, long double newImaginaryPart)
{
    long double maximum = std::numeric_limits<double>::max();

    if (!std::isfinite(newRealPart) || !std::isfinite(newImaginaryPart) ||
        std::abs(newRealPart) > maximum || std::abs(newImaginaryPart) > maximum ||
        std::hypot(newRealPart, newImaginaryPart) > maximum)
    {
        throw std::overflow_error("Результат операции не помещается в double");
    }

    double resultReal = static_cast<double>(newRealPart);
    double resultImaginary = static_cast<double>(newImaginaryPart);

    if (!std::isfinite(std::hypot(resultReal, resultImaginary)))
    {
        throw std::overflow_error("Модуль результата не помещается в double");
    }

    realPart = resultReal;
    imaginaryPart = resultImaginary;
}

/**
 * @brief Прибавляет другое комплексное число
 */
Complex& Complex::operator+=(const Complex& other)
{
    assignResult(static_cast<long double>(realPart) + other.realPart,
                 static_cast<long double>(imaginaryPart) + other.imaginaryPart);
    return *this;
}

/**
 * @brief Вычитает другое комплексное число
 */
Complex& Complex::operator-=(const Complex& other)
{
    assignResult(static_cast<long double>(realPart) - other.realPart,
                 static_cast<long double>(imaginaryPart) - other.imaginaryPart);
    return *this;
}

/**
 * @brief Умножает число на другое комплексное число
 */
Complex& Complex::operator*=(const Complex& other)
{
    long double a = realPart;
    long double b = imaginaryPart;
    long double c = other.realPart;
    long double d = other.imaginaryPart;

    assignResult(a * c - b * d, a * d + b * c);
    return *this;
}

/**
 * @brief Делит число на другое комплексное число
 */
Complex& Complex::operator/=(const Complex& other)
{
    if (other.realPart == 0.0 && other.imaginaryPart == 0.0)
    {
        throw std::domain_error("Деление на нулевое комплексное число");
    }

    long double a = realPart;
    long double b = imaginaryPart;
    long double c = other.realPart;
    long double d = other.imaginaryPart;
    long double denominator = c * c + d * d;

    assignResult((a * c + b * d) / denominator,
                 (b * c - a * d) / denominator);
    return *this;
}

/**
 * @brief Возвращает сумму двух комплексных чисел
 */
Complex Complex::operator+(const Complex& other) const
{
    Complex result(*this);
    result += other;
    return result;
}

/**
 * @brief Возвращает разность двух комплексных чисел
 */
Complex Complex::operator-(const Complex& other) const
{
    Complex result(*this);
    result -= other;
    return result;
}

/**
 * @brief Возвращает произведение двух комплексных чисел
 */
Complex Complex::operator*(const Complex& other) const
{
    Complex result(*this);
    result *= other;
    return result;
}

/**
 * @brief Возвращает частное двух комплексных чисел
 */
Complex Complex::operator/(const Complex& other) const
{
    Complex result(*this);
    result /= other;
    return result;
}

/**
 * @brief Сравнивает модули: меньше ли модуль текущего числа
 */
bool Complex::operator<(const Complex& other) const
{
    return getModulus() < other.getModulus();
}

/**
 * @brief Сравнивает модули: больше ли модуль текущего числа
 */
bool Complex::operator>(const Complex& other) const
{
    return other < *this;
}

/**
 * @brief Проверяет, что модуль текущего числа не больше другого
 */
bool Complex::operator<=(const Complex& other) const
{
    return !(*this > other);
}

/**
 * @brief Проверяет, что модуль текущего числа не меньше другого
 */
bool Complex::operator>=(const Complex& other) const
{
    return !(*this < other);
}

/**
 * @brief Сравнивает обе части комплексных чисел
 */
bool Complex::operator==(const Complex& other) const
{
    return realPart == other.realPart && imaginaryPart == other.imaginaryPart;
}

/**
 * @brief Проверяет неравенство комплексных чисел
 */
bool Complex::operator!=(const Complex& other) const
{
    return !(*this == other);
}

/**
 * @brief Возвращает действительную часть
 *
 * @return Действительная часть числа
 */
double Complex::getRealPart() const
{
    return realPart;
}

/**
 * @brief Возвращает мнимую часть
 *
 * @return Мнимая часть числа
 */
double Complex::getImaginaryPart() const
{
    return imaginaryPart;
}

/**
 * @brief Возвращает модуль комплексного числа
 *
 * @return Модуль числа
 */
double Complex::getModulus() const
{
    return std::hypot(realPart, imaginaryPart);
}

/**
 * @brief Возвращает количество существующих чисел
 *
 * @return Количество объектов Complex
 */
int Complex::getObjectCount()
{
    return objectCount;
}

/**
 * @brief Выводит комплексное число на экран
 */
void Complex::printInfo() const
{
    std::cout << realPart;

    if (imaginaryPart < 0.0)
    {
        std::cout << " - " << -imaginaryPart << "i\n";
    }
    else
    {
        std::cout << " + " << imaginaryPart << "i\n";
    }
}
