#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <sstream>
#include <windows.h>
#include "Complex.h"
#include "Calculator.h"
#include "Tests.h"

/**
 * @brief Сообщает об ошибке, если условие не выполнено
 *
 * @param condition Результат проверяемого условия
 * @param message Название проверки
 * @param errors Количество ошибок
 */
void check(bool condition, const std::string& message, int& errors)
{
    if (!condition)
    {
        std::cout << "Ошибка: " << message << '\n';
        ++errors;
    }
}

/**
 * @brief Проверяет отказ при неправильных частях числа
 *
 * @param realPart Действительная часть
 * @param imaginaryPart Мнимая часть
 * @return true если объект не создан и счетчик сохранен
 */
bool rejectsNumber(double realPart, double imaginaryPart)
{
    int count = Complex::getObjectCount();

    try
    {
        Complex wrong(realPart, imaginaryPart);
    }
    catch (const std::invalid_argument&)
    {
        return Complex::getObjectCount() == count;
    }

    return false;
}

/**
 * @brief Проверяет отказ унарной операции без изменения числа
 *
 * @param number Изменяемое число
 * @param operation Номер операции: 0 — ++a, 1 — a++, 2 — --a, 3 — a--
 * @return true если операция отклонена и состояние сохранено
 */
bool rejectsStep(Complex& number, int operation)
{
    double oldReal = number.getRealPart();
    double oldImaginary = number.getImaginaryPart();
    int count = Complex::getObjectCount();

    try
    {
        switch (operation)
        {
            case 0: ++number; break;
            case 1: number++; break;
            case 2: --number; break;
            case 3: number--; break;
            default: return false;
        }
    }
    catch (const std::overflow_error&)
    {
        return number.getRealPart() == oldReal &&
               number.getImaginaryPart() == oldImaginary &&
               Complex::getObjectCount() == count;
    }

    return false;
}

/**
 * @brief Проверяет унарные операции комплексного числа
 *
 * @param errors Количество ошибок
 */
void testUnaryOperations(int& errors)
{
    std::cout << "\nУнарные операции:\n";
    int initialCount = Complex::getObjectCount();

    {
        Complex number(3.0, 4.0);
        Complex& afterIncrement = ++number;
        check(&afterIncrement == &number && number.getRealPart() == 4.0 &&
              number.getImaginaryPart() == 4.0,
              "Префикс ++ изменил число и вернул ссылку на него", errors);
        check(Complex::getObjectCount() == initialCount + 1,
              "Префикс ++ не создал копию", errors);

        Complex beforeIncrement = number++;
        check(beforeIncrement.getRealPart() == 4.0 && beforeIncrement.getImaginaryPart() == 4.0 &&
              number.getRealPart() == 5.0 && number.getImaginaryPart() == 4.0,
              "Постфикс ++ вернул старое значение и увеличил число", errors);
        check(Complex::getObjectCount() == initialCount + 2,
              "Сохраненный результат постфикса считается отдельным объектом", errors);
        ++beforeIncrement;
        ++beforeIncrement;
        check(number.getRealPart() == 5.0 && beforeIncrement.getRealPart() == 6.0,
              "Изменение возвращенной копии не изменило исходное число", errors);

        Complex& afterDecrement = --number;
        check(&afterDecrement == &number && number.getRealPart() == 4.0 &&
              number.getImaginaryPart() == 4.0,
              "Префикс -- изменил число и вернул ссылку на него", errors);

        Complex beforeDecrement = number--;
        check(beforeDecrement.getRealPart() == 4.0 && beforeDecrement.getImaginaryPart() == 4.0 &&
              number.getRealPart() == 3.0 && number.getImaginaryPart() == 4.0,
              "Постфикс -- вернул старое значение и уменьшил число", errors);
        check(Complex::getObjectCount() == initialCount + 3,
              "Временные копии постфиксных операций уничтожены", errors);

        std::cout << "После ++a, a++, --a, a--: ";
        number.printInfo();
        ++(++number);
        check(number.getRealPart() == 5.0 && number.getImaginaryPart() == 4.0,
              "Префиксные операции можно выполнять последовательно", errors);
        --(--number);
        check(number.getRealPart() == 3.0 && number.getImaginaryPart() == 4.0,
              "Два уменьшения вернули исходное значение", errors);

        int beforeDiscardedResult = Complex::getObjectCount();
        number++;
        number--;
        check(Complex::getObjectCount() == beforeDiscardedResult && number.getRealPart() == 3.0,
              "Несохраненные результаты постфикса уничтожены", errors);
    }

    check(Complex::getObjectCount() == initialCount,
          "После унарных операций счетчик вернулся к исходному значению", errors);

    {
        Complex number(-1.0, -2.0);
        ++number;
        check(number.getRealPart() == 0.0 && number.getImaginaryPart() == -2.0,
              "Инкремент переводит действительную часть через ноль", errors);
        --number;
        check(number.getRealPart() == -1.0 && number.getImaginaryPart() == -2.0,
              "Декремент работает с отрицательными частями", errors);
    }

    std::cout << "\nНепредставимые изменения:\n";
    {
        Complex large(std::numeric_limits<double>::max(), 0.0);
        Complex negativeLarge(-std::numeric_limits<double>::max(), 0.0);

        for (int operation = 0; operation < 4; ++operation)
        {
            check(rejectsStep(large, operation),
                  "Операция с большим числом отклонена, данные и счетчик сохранены", errors);
            check(rejectsStep(negativeLarge, operation),
                  "Операция с большим отрицательным числом отклонена", errors);
        }
    }

    check(Complex::getObjectCount() == initialCount,
          "Ошибки унарных операций не оставили лишних объектов", errors);
}

/**
 * @brief Проверяет результат с учетом погрешности вычислений
 *
 * @param number Проверяемое число
 * @param realPart Ожидаемая действительная часть
 * @param imaginaryPart Ожидаемая мнимая часть
 * @return true если части совпадают с заданной точностью
 */
bool hasParts(const Complex& number, double realPart, double imaginaryPart)
{
    return std::abs(number.getRealPart() - realPart) < 0.000001 &&
           std::abs(number.getImaginaryPart() - imaginaryPart) < 0.000001;
}

/**
 * @brief Проверяет сохранение состояния при ошибке арифметики
 *
 * @param number Изменяемое число
 * @param other Второй операнд
 * @param operation Номер операции: 0 — +=, 1 — -=, 2 — *=, 3 — /=
 * @param divisionByZero Ожидается ли ошибка деления на ноль
 * @return true если получена нужная ошибка и данные сохранены
 */
bool rejectsArithmetic(Complex& number, const Complex& other,
                       int operation, bool divisionByZero = false)
{
    double oldReal = number.getRealPart();
    double oldImaginary = number.getImaginaryPart();
    double otherReal = other.getRealPart();
    double otherImaginary = other.getImaginaryPart();
    int count = Complex::getObjectCount();
    bool rejected = false;

    try
    {
        switch (operation)
        {
            case 0: number += other; break;
            case 1: number -= other; break;
            case 2: number *= other; break;
            case 3: number /= other; break;
            default: return false;
        }
    }
    catch (const std::domain_error&)
    {
        rejected = divisionByZero;
    }
    catch (const std::overflow_error&)
    {
        rejected = !divisionByZero;
    }

    return rejected && number.getRealPart() == oldReal &&
           number.getImaginaryPart() == oldImaginary &&
           other.getRealPart() == otherReal && other.getImaginaryPart() == otherImaginary &&
           Complex::getObjectCount() == count;
}

/**
 * @brief Проверяет арифметические присваивания
 *
 * @param errors Количество ошибок
 */
void testArithmeticAssignment(int& errors)
{
    std::cout << "\nАрифметические присваивания:\n";
    int initialCount = Complex::getObjectCount();

    {
        Complex number(3.0, 4.0);
        const Complex other(1.0, -2.0);
        int count = Complex::getObjectCount();

        check(&(number += other) == &number && hasParts(number, 4.0, 2.0),
              "+= сложило части и вернуло ссылку на число", errors);
        check(&(number -= other) == &number && hasParts(number, 3.0, 4.0),
              "-= вычло части и вернуло ссылку на число", errors);
        check(&(number *= other) == &number && hasParts(number, 11.0, -2.0),
              "*= выполнило комплексное умножение", errors);
        check(&(number /= other) == &number && hasParts(number, 3.0, 4.0),
              "/= выполнило комплексное деление", errors);
        check(hasParts(other, 1.0, -2.0), "Правый операнд не изменился", errors);
        check(Complex::getObjectCount() == count,
              "Арифметические присваивания не создали новых объектов", errors);
        std::cout << "После +=, -=, *=, /=: ";
        number.printInfo();

        number += number;
        check(hasParts(number, 6.0, 8.0), "Число можно прибавить к самому себе", errors);
        number -= number;
        check(hasParts(number, 0.0, 0.0), "Вычитание самого себя дает ноль", errors);
        number = other;
        number *= number;
        check(hasParts(number, -3.0, -4.0), "Умножение самого на себя учитывает старые части", errors);
        number /= number;
        check(hasParts(number, 1.0, 0.0), "Ненулевое число делится само на себя", errors);

        Complex zero;
        number *= zero;
        check(hasParts(number, 0.0, 0.0), "Умножение на ноль дает ноль", errors);
        number /= other;
        check(hasParts(number, 0.0, 0.0), "Ноль делится на ненулевое число", errors);
        check(rejectsArithmetic(number, zero, 3, true),
              "Деление нуля на ноль отклонено", errors);
        number = other;
        check(rejectsArithmetic(number, zero, 3, true),
              "Деление ненулевого числа на ноль отклонено", errors);
        check(rejectsArithmetic(zero, zero, 3, true),
              "Деление нулевого числа на себя не меняет данные", errors);

        Complex imaginary(0.0, 2.0);
        imaginary *= imaginary;
        check(hasParts(imaginary, -4.0, 0.0), "Квадрат мнимого числа действительный", errors);
        number = other;
        (number += other) -= other;
        check(hasParts(number, 1.0, -2.0), "Присваивания можно объединять в выражение", errors);

        double maximum = std::numeric_limits<double>::max();
        Complex large(maximum, 0.0);
        Complex negativeLarge(-maximum, 0.0);
        Complex twice(2.0, 0.0);
        Complex tiny(std::numeric_limits<double>::denorm_min(), 0.0);
        check(rejectsArithmetic(large, large, 0), "Переполнение += не изменило число", errors);
        check(rejectsArithmetic(large, negativeLarge, 1), "Переполнение -= не изменило число", errors);
        check(rejectsArithmetic(large, twice, 2), "Переполнение *= не изменило число", errors);
        check(rejectsArithmetic(large, tiny, 3), "Переполнение /= не изменило число", errors);

        Complex diagonal(maximum * 0.6, 0.0);
        const Complex vertical(0.0, maximum * 0.9);
        check(rejectsArithmetic(diagonal, vertical, 0),
              "Переполнение модуля отклонено, даже если части конечные", errors);

        large /= large;
        check(hasParts(large, 1.0, 0.0), "Большое число делится само на себя без переполнения квадратов", errors);
        tiny /= tiny;
        check(hasParts(tiny, 1.0, 0.0), "Очень маленькое число делится само на себя", errors);
    }

    check(Complex::getObjectCount() == initialCount,
          "После арифметических проверок счетчик восстановлен", errors);
}

/**
 * @brief Проверяет ошибку бинарной операции и сохранение операндов
 *
 * @param first Первый операнд
 * @param second Второй операнд
 * @param operation Номер операции: 0 — +, 1 — -, 2 — *, 3 — /
 * @param divisionByZero Ожидается ли ошибка деления на ноль
 * @return true если получена нужная ошибка и исходные числа сохранены
 */
bool rejectsBinary(const Complex& first, const Complex& second,
                   int operation, bool divisionByZero = false)
{
    double firstReal = first.getRealPart();
    double firstImaginary = first.getImaginaryPart();
    double secondReal = second.getRealPart();
    double secondImaginary = second.getImaginaryPart();
    int count = Complex::getObjectCount();
    bool rejected = false;

    try
    {
        switch (operation)
        {
            case 0: (void)(first + second); break;
            case 1: (void)(first - second); break;
            case 2: (void)(first * second); break;
            case 3: (void)(first / second); break;
            default: return false;
        }
    }
    catch (const std::domain_error&)
    {
        rejected = divisionByZero;
    }
    catch (const std::overflow_error&)
    {
        rejected = !divisionByZero;
    }

    return rejected && first.getRealPart() == firstReal &&
           first.getImaginaryPart() == firstImaginary &&
           second.getRealPart() == secondReal && second.getImaginaryPart() == secondImaginary &&
           Complex::getObjectCount() == count;
}

/**
 * @brief Проверяет бинарную арифметику
 *
 * @param errors Количество ошибок
 */
void testBinaryArithmetic(int& errors)
{
    std::cout << "\nБинарная арифметика:\n";
    int initialCount = Complex::getObjectCount();

    {
        const Complex first(3.0, 4.0);
        const Complex second(1.0, -2.0);
        int count = Complex::getObjectCount();
        {
            Complex sum = first + second;
            Complex difference = first - second;
            Complex product = first * second;
            Complex quotient = first / second;

            check(hasParts(sum, 4.0, 2.0), "Сумма равна 4 + 2i", errors);
            check(hasParts(difference, 2.0, 6.0), "Разность равна 2 + 6i", errors);
            check(hasParts(product, 11.0, -2.0), "Произведение равно 11 - 2i", errors);
            check(hasParts(quotient, -1.0, 2.0), "Частное равно -1 + 2i", errors);
            check(hasParts(first, 3.0, 4.0) && hasParts(second, 1.0, -2.0),
                  "Все бинарные операции сохранили исходные операнды", errors);
            check(Complex::getObjectCount() == count + 4,
                  "Четыре сохраненных результата являются отдельными объектами", errors);

            ++sum;
            check(hasParts(sum, 5.0, 2.0) && hasParts(first, 3.0, 4.0) &&
                  hasParts(second, 1.0, -2.0), "Изменение результата не изменило операнды", errors);
            std::cout << "Произведение: ";
            product.printInfo();
            std::cout << "Частное: ";
            quotient.printInfo();
        }

        check(Complex::getObjectCount() == count, "Сохраненные результаты уничтожены", errors);
        check(hasParts(first + first, 6.0, 8.0), "Сложение числа с самим собой", errors);
        check(hasParts(first - first, 0.0, 0.0), "Разность числа и самого себя", errors);
        check(hasParts(first * first, -7.0, 24.0), "Квадрат комплексного числа", errors);
        check(hasParts(first / first, 1.0, 0.0), "Деление ненулевого числа на себя", errors);
        check(hasParts((first + second) - second, 3.0, 4.0),
              "Вычитание второго слагаемого восстанавливает первое", errors);
        check(hasParts((first * second) / second, 3.0, 4.0),
              "Деление произведения восстанавливает первый множитель", errors);
        check(Complex::getObjectCount() == count, "Результаты составных выражений уничтожены", errors);

        const Complex zero;
        check(hasParts(first + zero, 3.0, 4.0) && hasParts(zero - first, -3.0, -4.0),
              "Сложение и вычитание с нулем", errors);
        check(hasParts(first * zero, 0.0, 0.0) && hasParts(zero / first, 0.0, 0.0),
              "Умножение на ноль и деление нуля", errors);
        check(rejectsBinary(first, zero, 3, true), "Бинарное деление на ноль отклонено", errors);
        check(rejectsBinary(zero, zero, 3, true), "Бинарное деление нуля на ноль отклонено", errors);

        double maximum = std::numeric_limits<double>::max();
        const Complex large(maximum, 0.0);
        const Complex negativeLarge(-maximum, 0.0);
        const Complex twice(2.0, 0.0);
        const Complex tiny(std::numeric_limits<double>::denorm_min(), 0.0);
        check(rejectsBinary(large, large, 0), "Переполнение суммы не изменило операнды и счетчик", errors);
        check(rejectsBinary(large, negativeLarge, 1), "Переполнение разности не изменило операнды и счетчик", errors);
        check(rejectsBinary(large, twice, 2), "Переполнение произведения не изменило операнды и счетчик", errors);
        check(rejectsBinary(large, tiny, 3), "Переполнение частного не изменило операнды и счетчик", errors);
        check(hasParts(large / large, 1.0, 0.0) && hasParts(tiny / tiny, 1.0, 0.0),
              "Бинарное деление работает на границах диапазона", errors);
    }

    check(Complex::getObjectCount() == initialCount,
          "После бинарной арифметики счетчик восстановлен", errors);
}

/**
 * @brief Проверяет сравнения модулей и равенство частей
 *
 * @param errors Количество ошибок
 */
void testComparison(int& errors)
{
    std::cout << "\nСравнения:\n";
    int initialCount = Complex::getObjectCount();

    {
        const Complex small(1.0, 0.0);
        const Complex large(3.0, 4.0);
        const Complex equal(3.0, 4.0);
        const Complex sameModulus(-3.0, 4.0);
        const Complex differentImaginary(3.0, -4.0);
        const Complex zero;
        int count = Complex::getObjectCount();

        check(small < large && !(large < small), "Оператор < сравнивает модули", errors);
        check(large > small && !(small > large), "Оператор > сравнивает модули", errors);
        check(small <= large && large <= equal && !(large <= small),
              "Оператор <= учитывает меньший и равный модули", errors);
        check(large >= small && large >= equal && !(small >= large),
              "Оператор >= учитывает больший и равный модули", errors);
        check(large == equal && !(large == small), "Оператор == сравнивает обе части", errors);
        check(large != small && !(large != equal), "Оператор != противоположен равенству", errors);
        check(large != sameModulus && large != differentImaginary,
              "Различие любой части означает неравенство", errors);
        check(!(large < sameModulus) && !(large > sameModulus) &&
              large <= sameModulus && large >= sameModulus,
              "Разные числа могут иметь одинаковый модуль", errors);
        check(large == large && !(large != large) && !(large < large) &&
              !(large > large) && large <= large && large >= large,
              "Все шесть сравнений числа с самим собой", errors);
        check(zero < small && zero <= zero && zero >= zero && zero == zero,
              "Сравнения с нулевым числом", errors);

        const Complex negative(-2.0, 0.0);
        check(negative > small, "Знак действительной части не определяет порядок модулей", errors);
        const Complex huge(std::numeric_limits<double>::max(), 0.0);
        const Complex tiny(std::numeric_limits<double>::denorm_min(), 0.0);
        check(huge > large && tiny > zero && tiny < small,
              "Сравнение очень больших и маленьких модулей", errors);
        check(Complex::getObjectCount() == count + 3 && hasParts(large, 3.0, 4.0) &&
              hasParts(sameModulus, -3.0, 4.0),
              "Сравнения не меняют числа и не создают копий", errors);
    }

    check(Complex::getObjectCount() == initialCount, "После сравнений счетчик восстановлен", errors);
}

/**
 * @brief Проверяет калькулятор, историю и ввод пользователя
 *
 * @param errors Количество ошибок
 */
void testCalculator(int& errors)
{
    std::cout << "\nКалькулятор и история:\n";
    int count = Complex::getObjectCount();
    {
        Calculator calculator;
        const Complex initial(3.0, 4.0);
        const Complex other(1.0, -2.0);
        const Complex zero;
        check(hasParts(calculator.getCurrent(), 0.0, 0.0) && calculator.getHistory().empty(),
              "Калькулятор начинает с нуля и пустой истории", errors);
        const std::string operations[] = {
            "+", "-", "*", "/", "+=", "-=", "*=", "/=",
            "++a", "a++", "--a", "a--", "<", ">", "<=", ">=", "==", "!="
        };
        const double realResults[] = {4, 2, 11, -1, 4, 2, 11, -1, 4, 4, 2, 2, 3, 3, 3, 3, 3, 3};
        const double imaginaryResults[] = {2, 6, -2, 2, 2, 6, -2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};

        for (int i = 0; i < 18; ++i)
        {
            calculator.enterNumber(initial);
            std::string entry = calculator.calculate(operations[i], other);
            check(hasParts(calculator.getCurrent(), realResults[i], imaginaryResults[i]) &&
                  calculator.getHistory().size() == static_cast<std::size_t>(i + 1) &&
                  calculator.getHistory().back() == entry,
                  "Калькулятор выполнил " + operations[i] + " и добавил запись", errors);
        }

        check(calculator.getHistory()[9].find("= 3 + 4i; текущее число: 4 + 4i") != std::string::npos &&
              calculator.getHistory()[11].find("= 3 + 4i; текущее число: 2 + 4i") != std::string::npos,
              "История постфикса содержит старый результат и новое число", errors);
        const std::string expectedComparisons[] = {"false", "true", "false", "true", "false", "true"};
        for (int i = 0; i < 6; ++i)
        {
            const std::string& entry = calculator.getHistory()[12 + i];
            check(entry.substr(entry.find(" = ") + 3) == expectedComparisons[i],
                  "История содержит правильный результат сравнения " + operations[12 + i], errors);
        }

        std::vector<std::string> savedHistory = calculator.getHistory();
        try
        {
            calculator.calculate("/", zero);
            check(false, "Деление на ноль отклонено в калькуляторе", errors);
        }
        catch (const std::domain_error&)
        {
            check(calculator.getHistory() == savedHistory && hasParts(calculator.getCurrent(), 3.0, 4.0),
                  "При делении на ноль история и текущее число сохранены", errors);
        }
        try
        {
            calculator.calculate("unknown", other);
            check(false, "Неизвестная операция отклонена", errors);
        }
        catch (const std::invalid_argument&)
        {
            check(calculator.getHistory() == savedHistory && hasParts(calculator.getCurrent(), 3.0, 4.0),
                  "Неизвестная операция не изменила калькулятор", errors);
        }
        calculator.enterNumber(Complex(std::numeric_limits<double>::max(), 0.0));
        try
        {
            calculator.calculate("*", Complex(2.0, 0.0));
            check(false, "Переполнение отклонено в калькуляторе", errors);
        }
        catch (const std::overflow_error&)
        {
            check(calculator.getHistory() == savedHistory &&
                  calculator.getCurrent().getRealPart() == std::numeric_limits<double>::max(),
                  "При переполнении история и текущее число сохранены", errors);
        }
        check(hasParts(initial, 3.0, 4.0) && hasParts(other, 1.0, -2.0),
              "Калькулятор не изменил переданные ему числа", errors);
    }

    {
        std::istringstream input("3\n1\nwrong\n1 2 extra\n3 4\n2\n+\n1 -2\n2\na++\n2\n/\n0 0\n2\n???\n9\n3\n0\n");
        std::ostringstream output;
        runCalculator(input, output);
        std::string text = output.str();
        check(text.find("История пуста") != std::string::npos, "Меню показывает пустую историю", errors);
        check(text.find("Ошибка ввода: нужны два конечных числа") != std::string::npos &&
              text.find("Ошибка ввода: лишние данные") != std::string::npos,
              "После неправильного ввода можно ввести число заново", errors);
        check(text.find("1. (3 + 4i) + (1 - 2i) = 4 + 2i") != std::string::npos &&
              text.find("2. (4 + 2i) a++ = 4 + 2i; текущее число: 5 + 2i") != std::string::npos,
              "Меню выводит историю в порядке выполнения", errors);
        check(text.find("Ошибка: Деление на нулевое комплексное число") != std::string::npos &&
              text.find("Текущее число: 5 + 2i") != std::string::npos,
              "После ошибки деления меню продолжает работу", errors);
        check(text.find("Ошибка: неизвестная операция") != std::string::npos &&
              text.find("Ошибка: неизвестный пункт меню") != std::string::npos,
              "Меню обрабатывает неизвестные команды", errors);
    }
    {
        std::istringstream input("1\n");
        std::ostringstream output;
        runCalculator(input, output);
        check(input.eof(), "Конец ввода во время чтения числа завершает калькулятор", errors);
    }
    check(Complex::getObjectCount() == count, "После калькулятора не осталось объектов", errors);
}

/**
 * @brief Проверяет конструкторы, операции и время жизни чисел
 *
 * @return 0 если все проверки прошли, иначе 1
 */
int runTests()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int errors{};
    std::cout << "Лабораторная работа №5. Вариант 4\n\n";
    check(Complex::getObjectCount() == 0, "До создания чисел счетчик равен 0", errors);

    {
        Complex first;
        Complex second(3.0, 4.0);
        Complex third(second);

        std::cout << "\nНачальные числа:\n";
        first.printInfo();
        second.printInfo();
        third.printInfo();
        std::cout << '\n';

        check(Complex::getObjectCount() == 3, "Созданы три числа разными конструкторами", errors);
        check(first.getRealPart() == 0.0 && first.getImaginaryPart() == 0.0,
              "Число без аргументов равно нулю", errors);
        check(second.getRealPart() == 3.0 && second.getImaginaryPart() == 4.0,
              "Параметризованный конструктор сохранил обе части", errors);
        check(third.getRealPart() == second.getRealPart() &&
              third.getImaginaryPart() == second.getImaginaryPart(),
              "Конструктор копирования сохранил данные", errors);

        const Complex& constNumber = second;
        check(constNumber.getModulus() == 5.0, "Модуль числа 3 + 4i равен 5", errors);
        check(first.getModulus() == 0.0, "Модуль нулевого числа равен 0", errors);

        {
            Complex negative(-3.0, -4.0);
            std::cout << "Число с отрицательными частями: ";
            negative.printInfo();
            check(negative.getModulus() == 5.0, "Модуль не зависит от знаков частей", errors);
            check(Complex::getObjectCount() == 4, "Внутри блока существуют четыре числа", errors);

            third = negative;
            check(third.getRealPart() == -3.0 && third.getImaginaryPart() == -4.0,
                  "Присваивание скопировало данные", errors);
            check(Complex::getObjectCount() == 4, "Присваивание не создало новый объект", errors);
            check(second.getRealPart() == 3.0 && second.getImaginaryPart() == 4.0,
                  "Изменение копии не изменило оригинал", errors);
        }

        check(Complex::getObjectCount() == 3, "После выхода из внутреннего блока осталось три числа", errors);

        std::cout << "\nНеправильные данные:\n";
        double infinity = std::numeric_limits<double>::infinity();
        double notNumber = std::numeric_limits<double>::quiet_NaN();
        double maximum = std::numeric_limits<double>::max();
        check(rejectsNumber(infinity, 0.0), "Бесконечная действительная часть отклонена", errors);
        check(rejectsNumber(0.0, -infinity), "Бесконечная мнимая часть отклонена", errors);
        check(rejectsNumber(notNumber, 0.0), "NaN в действительной части отклонен", errors);
        check(rejectsNumber(0.0, notNumber), "NaN в мнимой части отклонен", errors);
        check(rejectsNumber(maximum, maximum), "Переполнение модуля отклонено", errors);
        check(Complex::getObjectCount() == 3, "Неудачные конструкторы не изменили счетчик", errors);

        {
            Complex large(maximum, 0.0);
            check(std::isfinite(large.getModulus()) && large.getModulus() == maximum,
                  "Максимальное конечное число с нулевой мнимой частью допустимо", errors);
        }
    }

    testUnaryOperations(errors);
    testArithmeticAssignment(errors);
    testBinaryArithmetic(errors);
    testComparison(errors);
    testCalculator(errors);
    check(Complex::getObjectCount() == 0, "Деструкторы уменьшили счетчик до 0", errors);
    if (errors == 0)
    {
        std::cout << "Все проверки выполнены\n";
    }
    else
    {
        std::cout << "Количество ошибок: " << errors << '\n';
    }
    return errors == 0 ? 0 : 1;
}
