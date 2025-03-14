@echo off
echo Copying demo.lua to build\Debug...
copy demo.lua build\Debug\demo.lua
copy server.lua build\Debug\server.lua

if %ERRORLEVEL% EQU 0 (
    echo Successfully copied demo.lua to build\Debug
) else (
    echo Failed to copy demo.lua
)
pause