#include <iostream>
#include <string>
#include <windows.h>
#include "Calculator.h"
#include "Tests.h"

/**
 * @brief Запускает одинаковый клиент для двух способов линковки
 * @param argc Количество аргументов
 * @param argv Аргументы командной строки
 * @return 0 при успешном выполнении, иначе 1
 */
int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    if (argc == 2 && std::string(argv[1]) == "--test")
    {
        return runTests();
    }
    if (argc != 1)
    {
        std::cout << "Без аргументов: калькулятор\n--test: автоматические проверки\n";
        return 1;
    }

    runCalculator(std::cin, std::cout);
    return Complex::getObjectCount() == 0 ? 0 : 1;
}
