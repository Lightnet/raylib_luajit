# raylib_luajit

# License: MIT

# Required:
 * VS2022 (Makefile build for LuaJIT)
 * CMake

# Information:

  Work in progress. This is just a test build.

  It took some time to debug and keep it simple.

  Using the CMake to build all binary application.

# Design:
  It run lua script entry instead of compile to binary application.
```
demo.lua 
```
  Current set for testing.

# Credit:
 * https://github.com/raysan5/raylib-lua
   * Help reference the setup easy.

# Notes:
 * Just AI Model to quickly debug and correct set up the build. But I had to trouble shoot the cmake path checks libs and VS2022 to build the LuaJIT libs to check.
 * LuaJIT need to be in VS2022 powershell dev. for the batch script to work.
 * Not all raylib cheatsheet and raymath are there yet. But should be easy I think.