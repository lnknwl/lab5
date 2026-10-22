#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <iosfwd>
#include <string>
#include <vector>
#include "Complex.h"

/**
 * @brief Калькулятор комплексных чисел с историей операций
 */
class LAB3_API Calculator
{
private:
    Complex current;
    std::vector<std::string> history;

public:
    /**
     * @brief Принимает новое текущее число, сохраняя историю
     *
     * @param number Введенное число
     */
    void enterNumber(const Complex& number);

    /**
     * @brief Возвращает текущее число
     *
     * @return Константная ссылка на число
     */
    const Complex& getCurrent() const;

    /**
     * @brief Возвращает историю успешных операций
     *
     * @return Константная ссылка на записи истории
     */
    const std::vector<std::string>& getHistory() const;

    /**
     * @brief Выполняет операцию и сохраняет её результат в истории
     *
     * @param operation Обозначение операции
     * @param other Второй операнд, для унарных операций не используется
     * @return Строка с операцией и результатом
     * @throws std::invalid_argument Если операция неизвестна
     * @throws std::domain_error При делении на ноль
     * @throws std::overflow_error Если результат нельзя сохранить
     */
    std::string calculate(const std::string& operation, const Complex& other);
};

/**
 * @brief Преобразует комплексное число в строку
 *
 * @param number Комплексное число
 * @return Строка с обеими частями числа
 */
LAB3_API std::string formatNumber(const Complex& number);

/**
 * @brief Запускает меню калькулятора
 *
 * @param input Поток ввода
 * @param output Поток вывода
 */
LAB3_API void runCalculator(std::istream& input, std::ostream& output);

#endif
