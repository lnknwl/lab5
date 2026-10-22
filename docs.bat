@echo off
setlocal
cd /d "%~dp0"
doxygen Doxyfile
if errorlevel 1 exit /b 1
if not exist docs\html\uml mkdir docs\html\uml
copy /y uml\*.puml docs\html\uml\ >nul
if errorlevel 1 exit /b 1
copy /y uml\*.svg docs\html\uml\ >nul
if errorlevel 1 exit /b 1
