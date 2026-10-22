#ifndef COMPLEX_H
#define COMPLEX_H

#include "Lab3Api.h"

/**
 * @brief Класс для хранения комплексного числа
 */
class LAB3_API Complex
{
private:
    double realPart;
    double imaginaryPart;
    static int objectCount;

    /**
     * @brief Изменяет действительную часть, проверяя результат
     *
     * @param change Изменение действительной части
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    void changeRealPart(double change);

    /**
     * @brief Проверяет и сохраняет результат арифметической операции
     *
     * @param newRealPart Новая действительная часть
     * @param newImaginaryPart Новая мнимая часть
     * @throws std::overflow_error Если части или модуль не помещаются в double
     */
    void assignResult(long double newRealPart, long double newImaginaryPart);

public:
    /**
     * @brief Создает комплексное число с нулевыми частями
     */
    Complex();

    /**
     * @brief Создает комплексное число с заданными частями
     *
     * @param realPart Действительная часть
     * @param imaginaryPart Мнимая часть
     * @throws std::invalid_argument Если части или модуль не являются конечными
     */
    Complex(double realPart, double imaginaryPart);

    /**
     * @brief Создает копию комплексного числа
     *
     * @param other Число для копирования
     */
    Complex(const Complex& other);

    /**
     * @brief Копирует данные в существующее число
     *
     * @param other Число для копирования
     * @return Ссылка на измененный объект
     */
    Complex& operator=(const Complex& other);

    /**
     * @brief Завершает время жизни числа и уменьшает счетчик
     */
    ~Complex();

    /**
     * @brief Увеличивает действительную часть на один
     *
     * @return Ссылка на измененное число
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex& operator++();

    /**
     * @brief Увеличивает действительную часть и возвращает старое число
     *
     * @param unused Признак постфиксной формы, значение не используется
     * @return Копия числа до увеличения
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex operator++(int unused);

    /**
     * @brief Уменьшает действительную часть на один
     *
     * @return Ссылка на измененное число
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex& operator--();

    /**
     * @brief Уменьшает действительную часть и возвращает старое число
     *
     * @param unused Признак постфиксной формы, значение не используется
     * @return Копия числа до уменьшения
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex operator--(int unused);

    /**
     * @brief Прибавляет другое комплексное число
     *
     * @param other Прибавляемое число
     * @return Ссылка на измененное число
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex& operator+=(const Complex& other);

    /**
     * @brief Вычитает другое комплексное число
     *
     * @param other Вычитаемое число
     * @return Ссылка на измененное число
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex& operator-=(const Complex& other);

    /**
     * @brief Умножает число на другое комплексное число
     *
     * @param other Второй множитель
     * @return Ссылка на измененное число
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex& operator*=(const Complex& other);

    /**
     * @brief Делит число на другое комплексное число
     *
     * @param other Делитель
     * @return Ссылка на измененное число
     * @throws std::domain_error При делении на нулевое число
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex& operator/=(const Complex& other);

    /**
     * @brief Возвращает сумму двух комплексных чисел
     *
     * @param other Второе слагаемое
     * @return Новое число с результатом сложения
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex operator+(const Complex& other) const;

    /**
     * @brief Возвращает разность двух комплексных чисел
     *
     * @param other Вычитаемое число
     * @return Новое число с результатом вычитания
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex operator-(const Complex& other) const;

    /**
     * @brief Возвращает произведение двух комплексных чисел
     *
     * @param other Второй множитель
     * @return Новое число с результатом умножения
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex operator*(const Complex& other) const;

    /**
     * @brief Возвращает частное двух комплексных чисел
     *
     * @param other Делитель
     * @return Новое число с результатом деления
     * @throws std::domain_error При делении на нулевое число
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    Complex operator/(const Complex& other) const;

    /**
     * @brief Сравнивает модули: меньше ли модуль текущего числа
     *
     * @param other Число для сравнения
     * @return true если модуль текущего числа меньше
     */
    bool operator<(const Complex& other) const;

    /**
     * @brief Сравнивает модули: больше ли модуль текущего числа
     *
     * @param other Число для сравнения
     * @return true если модуль текущего числа больше
     */
    bool operator>(const Complex& other) const;

    /**
     * @brief Проверяет, что модуль текущего числа не больше другого
     *
     * @param other Число для сравнения
     * @return true если модуль меньше или равен
     */
    bool operator<=(const Complex& other) const;

    /**
     * @brief Проверяет, что модуль текущего числа не меньше другого
     *
     * @param other Число для сравнения
     * @return true если модуль больше или равен
     */
    bool operator>=(const Complex& other) const;

    /**
     * @brief Сравнивает обе части комплексных чисел
     *
     * @param other Число для сравнения
     * @return true если действительные и мнимые части равны
     */
    bool operator==(const Complex& other) const;

    /**
     * @brief Проверяет неравенство комплексных чисел
     *
     * @param other Число для сравнения
     * @return true если хотя бы одна часть отличается
     */
    bool operator!=(const Complex& other) const;

    /**
     * @brief Возвращает действительную часть
     *
     * @return Действительная часть числа
     */
    double getRealPart() const;

    /**
     * @brief Возвращает мнимую часть
     *
     * @return Мнимая часть числа
     */
    double getImaginaryPart() const;

    /**
     * @brief Возвращает модуль комплексного числа
     *
     * @return Модуль числа
     */
    double getModulus() const;

    /**
     * @brief Возвращает количество существующих чисел
     *
     * @return Количество объектов Complex
     */
    static int getObjectCount();

    /**
     * @brief Выводит комплексное число на экран
     */
    void printInfo() const;
};

#endif
