@echo off
chcp 65001 >nul
setlocal
cd /d "%~dp0"

if not exist build\static mkdir build\static
if not exist build\shared mkdir build\shared

set "FLAGS=-std=c++14 -Wall -Wextra -pedantic -finput-charset=UTF-8 -fexec-charset=CP1251 -Iinclude -Itests"

rem Статическая библиотека
g++ %FLAGS% -c src\Complex.cpp -o build\static\Complex.o
if errorlevel 1 exit /b 1
g++ %FLAGS% -c src\Calculator.cpp -o build\static\Calculator.o
if errorlevel 1 exit /b 1
ar rcs build\static\liblab3.a build\static\Complex.o build\static\Calculator.o
if errorlevel 1 exit /b 1

rem Динамическая библиотека и библиотека импорта
g++ %FLAGS% -DLAB3_SHARED -DLAB3_BUILD -c src\Complex.cpp -o build\shared\Complex.o
if errorlevel 1 exit /b 1
g++ %FLAGS% -DLAB3_SHARED -DLAB3_BUILD -c src\Calculator.cpp -o build\shared\Calculator.o
if errorlevel 1 exit /b 1
g++ -shared build\shared\Complex.o build\shared\Calculator.o -Wl,--out-implib,build\shared\lab3.lib -o build\shared\lab3.dll
if errorlevel 1 exit /b 1

rem Один клиентский код, разные библиотеки
g++ %FLAGS% -c clients\main.cpp -o build\static\main.o
if errorlevel 1 exit /b 1
g++ %FLAGS% -c tests\Tests.cpp -o build\static\Tests.o
if errorlevel 1 exit /b 1
g++ build\static\main.o build\static\Tests.o build\static\liblab3.a -o build\static\lab5-static.exe
if errorlevel 1 exit /b 1

g++ %FLAGS% -DLAB3_SHARED -c clients\main.cpp -o build\shared\main.o
if errorlevel 1 exit /b 1
g++ %FLAGS% -DLAB3_SHARED -c tests\Tests.cpp -o build\shared\Tests.o
if errorlevel 1 exit /b 1
g++ build\shared\main.o build\shared\Tests.o build\shared\lab3.lib -o build\shared\lab5-dynamic.exe
if errorlevel 1 exit /b 1

rem Библиотеки среды MinGW для запуска без изменения PATH
set "COMPILER_DIR="
for /f "delims=" %%C in ('where g++.exe') do if not defined COMPILER_DIR set "COMPILER_DIR=%%~dpC"
if not defined COMPILER_DIR exit /b 1
copy /y "%COMPILER_DIR%libstdc++-6.dll" build\static\ >nul
if errorlevel 1 exit /b 1
copy /y "%COMPILER_DIR%libgcc_s_seh-1.dll" build\static\ >nul
if errorlevel 1 exit /b 1
copy /y "%COMPILER_DIR%libwinpthread-1.dll" build\static\ >nul
if errorlevel 1 exit /b 1
copy /y "%COMPILER_DIR%libstdc++-6.dll" build\shared\ >nul
if errorlevel 1 exit /b 1
copy /y "%COMPILER_DIR%libgcc_s_seh-1.dll" build\shared\ >nul
if errorlevel 1 exit /b 1
copy /y "%COMPILER_DIR%libwinpthread-1.dll" build\shared\ >nul
if errorlevel 1 exit /b 1

echo Сборка выполнена успешно
