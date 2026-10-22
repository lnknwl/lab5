# Ручная сборка

Команды выполняются из папки lab5. Нужен MinGW-w64 UCRT64 для Windows x64. Все части проекта собираются прямыми вызовами g++ и ar. build.bat просто выполняет эти команды по порядку и останавливается при ошибке

## Компиляция и линковка

Ниже команды для cmd. Переменная FLAGS содержит общие параметры компилятора

```bat
if not exist build\static mkdir build\static
if not exist build\shared mkdir build\shared
set "FLAGS=-std=c++14 -Wall -Wextra -pedantic -finput-charset=UTF-8 -fexec-charset=CP1251 -Iinclude -Itests"

g++ %FLAGS% -c src\Complex.cpp -o build\static\Complex.o
g++ %FLAGS% -c src\Calculator.cpp -o build\static\Calculator.o
ar rcs build\static\liblab3.a build\static\Complex.o build\static\Calculator.o

g++ %FLAGS% -DLAB3_SHARED -DLAB3_BUILD -c src\Complex.cpp -o build\shared\Complex.o
g++ %FLAGS% -DLAB3_SHARED -DLAB3_BUILD -c src\Calculator.cpp -o build\shared\Calculator.o
g++ -shared build\shared\Complex.o build\shared\Calculator.o -Wl,--out-implib,build\shared\lab3.lib -o build\shared\lab3.dll

g++ %FLAGS% -c clients\main.cpp -o build\static\main.o
g++ %FLAGS% -c tests\Tests.cpp -o build\static\Tests.o
g++ build\static\main.o build\static\Tests.o build\static\liblab3.a -o build\static\lab5-static.exe

g++ %FLAGS% -DLAB3_SHARED -c clients\main.cpp -o build\shared\main.o
g++ %FLAGS% -DLAB3_SHARED -c tests\Tests.cpp -o build\shared\Tests.o
g++ build\shared\main.o build\shared\Tests.o build\shared\lab3.lib -o build\shared\lab5-dynamic.exe
```

Ключ -c создает объектный файл без линковки. ar объединяет объектные файлы в статический архив. Ключ -shared создает DLL, а --out-implib записывает библиотеку импорта

Объектные файлы клиента указаны перед библиотекой. Реализации классов не добавляются напрямую в команды линковки exe: клиент должен получить их именно из соответствующей библиотеки

Для DLL и статического архива используются разные объектные файлы, поскольку макрос LAB3_API раскрывается по-разному

## Файлы для запуска

Рядом с обоими exe нужны библиотеки среды MinGW:

- libstdc++-6.dll
- libgcc_s_seh-1.dll
- libwinpthread-1.dll

build.bat берет их из каталога g++.exe и копирует в build/static и build/shared. Для динамического клиента дополнительно нужна наша lab3.dll, которая уже создается в build/shared

Для запуска статического клиента не нужны liblab3.a и объектные файлы. Для запуска динамического клиента не нужна библиотека импорта lab3.lib: она используется только при линковке

Все файлы собраны одной версией компилятора. Заголовки и DLL должны соответствовать друг другу. Файлы сборки и HTML-документация исключены из Git, их можно получить заново

## Исходная работа

Complex и Calculator скопированы из лабораторной №3, коммит b73ef8ea3852fe24e1af7acdf0e4d91992798a22. В копиях заголовков добавлен экспорт классов и свободных функций, определение присваивания Complex перенесено в .cpp. Автоматические проверки отделены от клиентского main()

Сборка не обращается к соседней папке lab3. Папку lab5 можно перенести отдельно
