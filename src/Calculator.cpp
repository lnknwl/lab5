#include <iomanip>
#include <istream>
#include <limits>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include "Calculator.h"

/**
 * @brief Преобразует комплексное число в строку
 */
std::string formatNumber(const Complex& number)
{
    std::ostringstream text;
    text << std::setprecision(std::numeric_limits<double>::max_digits10);
    text << number.getRealPart();

    if (number.getImaginaryPart() < 0.0)
    {
        text << " - " << -number.getImaginaryPart() << 'i';
    }
    else
    {
        text << " + " << number.getImaginaryPart() << 'i';
    }

    return text.str();
}

/**
 * @brief Принимает новое текущее число, сохраняя историю
 */
void Calculator::enterNumber(const Complex& number)
{
    current = number;
}

/**
 * @brief Возвращает текущее число
 */
const Complex& Calculator::getCurrent() const
{
    return current;
}

/**
 * @brief Возвращает историю успешных операций
 */
const std::vector<std::string>& Calculator::getHistory() const
{
    return history;
}

/**
 * @brief Выполняет операцию и сохраняет её результат в истории
 */
std::string Calculator::calculate(const std::string& operation, const Complex& other)
{
    Complex result(current);
    std::string resultText;
    bool unary = operation == "++a" || operation == "a++" ||
                 operation == "--a" || operation == "a--";

    if (operation == "+") result = current + other;
    else if (operation == "-") result = current - other;
    else if (operation == "*") result = current * other;
    else if (operation == "/") result = current / other;
    else if (operation == "+=") result += other;
    else if (operation == "-=") result -= other;
    else if (operation == "*=") result *= other;
    else if (operation == "/=") result /= other;
    else if (operation == "++a") ++result;
    else if (operation == "--a") --result;
    else if (operation == "a++") resultText = formatNumber(result++);
    else if (operation == "a--") resultText = formatNumber(result--);
    else if (operation == "<") resultText = current < other ? "true" : "false";
    else if (operation == ">") resultText = current > other ? "true" : "false";
    else if (operation == "<=") resultText = current <= other ? "true" : "false";
    else if (operation == ">=") resultText = current >= other ? "true" : "false";
    else if (operation == "==") resultText = current == other ? "true" : "false";
    else if (operation == "!=") resultText = current != other ? "true" : "false";
    else throw std::invalid_argument("Неизвестная операция");

    std::string entry = "(" + formatNumber(current) + ") " + operation;
    if (!unary)
    {
        entry += " (" + formatNumber(other) + ")";
    }
    if (resultText.empty())
    {
        resultText = formatNumber(result);
    }
    entry += " = " + resultText;
    if (operation == "a++" || operation == "a--")
    {
        entry += "; текущее число: " + formatNumber(result);
    }

    history.push_back(entry);
    current = result;
    return entry;
}

namespace
{
/**
 * @brief Читает две части числа с проверкой ввода
 *
 * @param input Поток ввода
 * @param output Поток вывода
 * @param number Число для сохранения результата
 * @return false если ввод завершен
 */
bool readNumber(std::istream& input, std::ostream& output, Complex& number)
{
    std::string line;
    while (true)
    {
        output << "Введите действительную и мнимую части через пробел: " << std::flush;
        if (!std::getline(input, line)) return false;

        std::istringstream values(line);
        double realPart{};
        double imaginaryPart{};
        if (!(values >> realPart >> imaginaryPart))
        {
            output << "Ошибка ввода: нужны два конечных числа\n";
            continue;
        }
        values >> std::ws;
        if (!values.eof())
        {
            output << "Ошибка ввода: лишние данные\n";
            continue;
        }

        try
        {
            number = Complex(realPart, imaginaryPart);
            return true;
        }
        catch (const std::invalid_argument& error)
        {
            output << "Ошибка ввода: " << error.what() << '\n';
        }
    }
}
}

/**
 * @brief Запускает меню калькулятора
 */
void runCalculator(std::istream& input, std::ostream& output)
{
    Calculator calculator;
    std::string choice;
    output << "Калькулятор комплексных чисел\n";

    while (true)
    {
        output << "\nТекущее число: " << formatNumber(calculator.getCurrent()) << '\n';
        output << "1 - Ввести число\n2 - Выполнить операцию\n3 - История\n0 - Выход\nВыберите действие: " << std::flush;
        if (!std::getline(input, choice) || choice == "0") break;

        if (choice == "1")
        {
            Complex number;
            if (!readNumber(input, output, number)) break;
            calculator.enterNumber(number);
        }
        else if (choice == "2")
        {
            output << "Операция: + - * / += -= *= /= ++a a++ --a a-- < > <= >= == !=\n";
            std::string operation;
            if (!std::getline(input, operation)) break;
            bool unary = operation == "++a" || operation == "a++" ||
                         operation == "--a" || operation == "a--";
            bool binary = operation == "+" || operation == "-" || operation == "*" ||
                          operation == "/" || operation == "+=" || operation == "-=" ||
                          operation == "*=" || operation == "/=" || operation == "<" ||
                          operation == ">" || operation == "<=" || operation == ">=" ||
                          operation == "==" || operation == "!=";
            if (!unary && !binary)
            {
                output << "Ошибка: неизвестная операция\n";
                continue;
            }

            Complex other;
            if (binary && !readNumber(input, output, other)) break;
            try
            {
                output << calculator.calculate(operation, other) << '\n';
            }
            catch (const std::logic_error& error)
            {
                output << "Ошибка: " << error.what() << '\n';
            }
            catch (const std::overflow_error& error)
            {
                output << "Ошибка: " << error.what() << '\n';
            }
        }
        else if (choice == "3")
        {
            output << "История операций:\n";
            if (calculator.getHistory().empty()) output << "История пуста\n";
            for (std::size_t i = 0; i < calculator.getHistory().size(); ++i)
            {
                output << i + 1 << ". " << calculator.getHistory()[i] << '\n';
            }
        }
        else
        {
            output << "Ошибка: неизвестный пункт меню\n";
        }
    }
}
