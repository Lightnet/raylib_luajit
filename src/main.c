#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <raylib.h>
#include <stdio.h>
#include "raylib-luajit.h"

int main(int argc, char *argv[]) {
    // Initialize LuaJIT state
    lua_State *L = luaL_newstate();
    if (!L) {
        fprintf(stderr, "Failed to create LuaJIT state\n");
        return 1;
    }

    // Open standard Lua libraries
    luaL_openlibs(L);

    // Register raylib functions with Lua
    luaopen_raylib(L);
    lua_setglobal(L, "rl");  // Accessible as 'rl' in Lua

    // Load and execute the Lua script
    const char *script_path = "demo.lua";
    if (luaL_loadfile(L, script_path) || lua_pcall(L, 0, 0, 0)) {
        fprintf(stderr, "Error running script: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // Clean up
    lua_close(L);
    return 0;
}