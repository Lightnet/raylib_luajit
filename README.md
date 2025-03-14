# raylib_luajit

# License: MIT

# Required:
 * VS2022 (Makefile build for LuaJIT)
 * CMake

# Libs:
 * raylib 5.5 and raymath (wip wrapper api)
 * raygui 4.0 (wip wrapper api)
 * zpl-c/enet 2.6.2 (dll not working correctly)
 * luajit 2.1.0 beta3


# Information:

  Work in progress. This is just a test build. It took some time to debug and keep it simple.

  Using the CMake to build all in one binary application to run off lua script.

  There are two way to run lua script for raylib. One using the wrapper which required some setup and other is c library loader from script which raylib need to be added to the main.c to access but required setup functions and vars.

  For FFI to work need c dll library. As it need the name to load and use the api calls.

# Design:
  Run lua script entry point instead of compile to binary application every time. Reduce. But depend on the code build.
```
demo.lua 
```
  Current set for testing.

# Credit:
 * https://github.com/raysan5/raylib-lua
   * Help reference the setup easy.

# Lua Script Notes:
  Create a api lua to define api as rl = raylib for easy access. It can be found in raylib-luajit.h which required register functions and vars. To run lua script easy.

  Using required package for standard. Not using global. Reason chance it get overrided.

```c
// Register raylib module in package.preload
lua_getglobal(L, "package");
lua_getfield(L, -1, "preload");
lua_pushcfunction(L, luaopen_raylib);
lua_setfield(L, -2, "rl");
lua_pushcfunction(L, luaopen_enet);
lua_setfield(L, -2, "enet");
lua_pop(L, 2);  // Remove package and preload from stack
```

```lua
local rl = require("rl")
local enet = require("enet")
```

## Not correct setup.
  Just the testing builds.
```c
//main.c
// Register raylib functions with Lua
luaopen_raylib(L);
lua_setglobal(L, "rl");  // Accessible as 'rl' in Lua
```

```lua
rl.InitWindow(800, 450, "raylib [core] demo - LuaJIT")
rl.SetTargetFPS(60)  -- Cap at 60 FPS
while not rl.WindowShouldClose() do
  local dt = rl.GetFrameTime()
  rl.BeginDrawing()
    rl.ClearBackground(rl.RAYWHITE)
  rl.EndDrawing()
end
rl.CloseWindow()
```

## FFI Library:
  Another way is access c library using FFI for raylib and other libs. Example filename.dll

```lua
local ffi = require("ffi")

ffi.cdef[[
    void InitWindow(int width, int height, const char *title);
    void CloseWindow(void);
    void BeginDrawing(void);
    void EndDrawing(void);
    void ClearBackground(unsigned int color);
]]
local raylib = ffi.C --not right format error.
```
The reason is not correct way to load raylib api.

```lua
local ffi = require("ffi")

ffi.cdef[[
    void InitWindow(int width, int height, const char *title);
    void CloseWindow(void);
    void BeginDrawing(void);
    void EndDrawing(void);
    void ClearBackground(unsigned int color);
]]
local raylib = ffi.load(ffi.os == "Windows" and "raylib" or "raylib") --correct way to load lib
raylib.InitWindow(800, 450, "raylib [core] demo - LuaJIT")
```
 * https://luajit.org/ext_ffi_api.html
    * clib = ffi.load(name [,global])
 This is correct way to load library for raylib to access their api.


# Notes:
 * Just AI Model to quickly debug and correct set up the build. But I had to trouble shoot the cmake path checks libs and VS2022 to build the LuaJIT libs to check.
 * LuaJIT need to be in VS2022 powershell dev. for the batch script to work.
 * Not all raylib cheatsheet and raymath are there yet. But should be easy I think.
 * Can access raylib from FFI Library load.