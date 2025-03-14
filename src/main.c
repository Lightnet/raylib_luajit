#define WIN32_LEAN_AND_MEAN 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
//#define NOSOUND           // Sound driver routines
#define MMNOSOUND

#define ENET_IMPLEMENTATION
#include <enet.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <raylib.h>
#include <stdio.h>
#include <string.h>

#include "raylib-luajit.h"
#include "enet-luajit.h"

int main(int argc, char *argv[]) {
    // Default script path
    const char *script_path = "demo.lua";

    // Check if a script path is provided as a command-line argument
    if (argc > 1) {
        script_path = argv[1];
        const char *ext = strrchr(script_path, '.');
        if (!ext || strcmp(ext, ".lua") != 0) {
            fprintf(stderr, "Error: Script '%s' must have a .lua extension\n", script_path);
            return 1;
        }
    }

    // Initialize LuaJIT state
    lua_State *L = luaL_newstate();
    if (!L) {
        fprintf(stderr, "Failed to create LuaJIT state\n");
        return 1;
    }

    // Open standard Lua libraries
    luaL_openlibs(L);

    // Register raylib module in package.preload
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_pushcfunction(L, luaopen_raylib);
    lua_setfield(L, -2, "rl");
    lua_pushcfunction(L, luaopen_enet);
    lua_setfield(L, -2, "enet");
    lua_pop(L, 2);  // Remove package and preload from stack

    // Load and execute the Lua script
    if (luaL_loadfile(L, script_path) || lua_pcall(L, 0, 0, 0)) {
        fprintf(stderr, "Error running script '%s': %s\n", script_path, lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // Clean up
    lua_close(L);
    return 0;
}