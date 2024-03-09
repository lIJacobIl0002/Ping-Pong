@echo off
g++ main.cpp -o main.exe -lgdi32
if %errorlevel% equ 0 (
    main.exe
) else (
    echo Compilation failed
)

