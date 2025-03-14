@echo off
setlocal
set APPNAME=raylib_luajit
set APPPATH=build\Debug\%APPNAME%.exe
set EXECUTABLE=%APPNAME%.exe

if not exist %APPPATH% (
    echo Executable not found! Please build the project first.
    exit /b 1
)

echo Running raylib_luajit...

cd build\Debug\

%EXECUTABLE% demo.lua

if %ERRORLEVEL% NEQ 0 (
    echo Program exited with error code %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)

echo Program ran successfully!
endlocal