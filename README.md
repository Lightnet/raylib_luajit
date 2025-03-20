# raylib_luajit

A simple project to run raylib, raygui, enet, and raymath from Lua scripts using LuaJIT, all in one executable.

# License:
- MIT License - Free to use, modify, and share!
    

---

# What You Need
- Visual Studio 2022: For building LuaJIT (uses a Makefile).
- CMake: To generate the build files and compile everything.

---

# Libraries Used
- raylib 5.5: Core library for graphics, input, and more. Includes raymath (work-in-progress Lua wrapper).
- raygui 4.0: Simple GUI library for raylib (work-in-progress Lua wrapper).
- enet 2.6.2: Networking library (DLL support is buggy right now).
- LuaJIT 2.1.0-beta3: Fast Lua runtime to run scripts.

---

# About This Project

This is a work-in-progress test to combine raylib and other libraries into a single executable that runs Lua scripts. It’s designed to be simple and avoid recompiling C code every time—just write Lua and run!

Why?
- Faster Workflow: Edit Lua scripts and run them directly with raylib_luajit.exe.
- Learning Tool: A starting point to play with raylib, networking, and GUI in Lua.

Current Status
- It works! See demo.lua for a test with graphics, networking, and GUI.
- Still debugging and adding features (e.g., more raymath functions).
---

# How It Works

We use CMake to build one binary (raylib_luajit.exe) that loads Lua scripts. There are two ways to use raylib in Lua:
1. Lua Wrapper (Recommended): Pre-built C-to-Lua bindings (e.g., rl.InitWindow).
2. FFI (Alternative): Load raylib’s C functions directly from Lua (needs a DLL).

## Design Goal
- Run Lua scripts as the entry point instead of rebuilding a binary each time.
- Keep C-to-Lua and Lua-to-C data (like colors, vectors) in a clear, matching format.

---

# Building It
1. Install Requirements:
    - Visual Studio 2022 (Community edition works).
    - CMake (download from cmake.org or install via package manager).
2. Clone the Repo:
    bash
    ```bash
    git clone <repo-url>
    cd raylib_luajit
    ```
    
3. Build with CMake:
    powershell
    ```powershell
    mkdir build
    cd build
    cmake .. -G "Visual Studio 17 2022" -A x64
    cmake --build . --config Debug
    ```
    
4. Run:
    - Server: .\Debug\raylib_luajit.exe ..\..\server.lua
    - Client: .\Debug\run.bat (runs demo.lua).
        

---

# Using Lua 

Wrapper Method (Recommended)

We’ve wrapped raylib, raygui, enet, and raymath in Lua modules (rl, rg, enet, rm). Load them with require:

lua
```lua
local rl = require("rl")
local enet = require("enet")

rl.InitWindow(800, 450, "My Game")
while not rl.WindowShouldClose() do
    rl.BeginDrawing()
    rl.ClearBackground(rl.RAYWHITE)  -- Predefined color
    rl.DrawRectangle(100, 200, 50, 50, {255, 0, 0, 255})  -- Custom color
    rl.EndDrawing()
end
rl.CloseWindow()
```

How It’s Set Up in C

In main.c, we register modules so require works:

c
```c
lua_getglobal(L, "package");
lua_getfield(L, -1, "preload");
lua_pushcfunction(L, luaopen_raylib);  lua_setfield(L, -2, "rl");
lua_pushcfunction(L, luaopen_enet);    lua_setfield(L, -2, "enet");
lua_pushcfunction(L, luaopen_raygui);  lua_setfield(L, -2, "rg");
lua_pushcfunction(L, luaopen_raymath); lua_setfield(L, -2, "rm");
lua_pop(L, 2);
```

- Why require? Avoids global variables (rl could be overwritten). Keeps code clean and modular.
---

# FFI Method (Alternative)

Use LuaJIT’s FFI (Foreign Function Interface) to call raylib C functions directly. You need the raylib DLL (raylib.dll) in the same folder.

Example

lua
```lua
local ffi = require("ffi")

ffi.cdef[[
    void InitWindow(int width, int height, const char *title);
    bool WindowShouldClose(void);
    void BeginDrawing(void);
    void EndDrawing(void);
    void ClearBackground(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
]]

local rl = ffi.load("raylib")  -- Load raylib.dll

rl.InitWindow(800, 450, "FFI Test")
while not rl.WindowShouldClose() do
    rl.BeginDrawing()
    rl.ClearBackground(245, 245, 245, 255)  -- RAYWHITE as separate args
    rl.EndDrawing()
end
```

# Notes
- Pros: Direct access to all raylib functions, no wrapper needed.
- Cons: Needs raylib.dll, trickier color/struct handling (no {r, g, b, a} tables).
---

# Lua Script Tips

Colors
- Predefined: Use rl.RAYWHITE, rl.RED, rl.GREEN, etc.
- Custom: Pass a table {r, g, b, a} (e.g., {128, 0, 128, 255} for purple).

Demo

Check demo.lua for a working example with:
- Moving rectangle (raylib + raymath).
- Networking (enet).
- GUI button, label, checkbox (raygui).
---

# Troubleshooting
- LuaJIT Build: Run in a VS2022 Developer PowerShell for msvcbuild.bat to work.
- CMake Errors: Ensure all libs (raylib, enet, etc.) downloaded via CPM (CMake handles this).
- Black Screen: Rebuild fully (Remove-Item -Recurse -Force * in build/ folder).

---

Credits
- raylib-lua: Inspired setup ([https://github.com/raysan5/raylib-lua](https://github.com/raysan5/raylib-lua)).
	- AI Help: Used Grok (xAI) to debug CMake and wrappers.

# Raylib CheatSheet:
 - [ ] Test

# RayMath CheatSheet:

# RayGui CheatSheet: