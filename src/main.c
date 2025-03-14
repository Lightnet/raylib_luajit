#define WIN32_LEAN_AND_MEAN 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define NOGDI
#define NOUSER
//#define NOSOUND
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
#include "raymath-luajit.h"
#include "raygui-luajit.h"

int main(int argc, char *argv[]) {
    const char *script_path = "demo.lua";

    if (argc > 1) {
        script_path = argv[1];
        const char *ext = strrchr(script_path, '.');
        if (!ext || strcmp(ext, ".lua") != 0) {
            fprintf(stderr, "Error: Script '%s' must have a .lua extension\n", script_path);
            return 1;
        }
    }

    lua_State *L = luaL_newstate();
    if (!L) {
        fprintf(stderr, "Failed to create LuaJIT state\n");
        return 1;
    }

    luaL_openlibs(L);

    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_pushcfunction(L, luaopen_raylib);
    lua_setfield(L, -2, "rl");
    lua_pushcfunction(L, luaopen_enet);
    lua_setfield(L, -2, "enet");
    lua_pushcfunction(L, luaopen_raymath);
    lua_setfield(L, -2, "rm");
    lua_pushcfunction(L, luaopen_raygui);
    lua_setfield(L, -2, "rg");
    lua_pop(L, 2);

    if (luaL_loadfile(L, script_path) || lua_pcall(L, 0, 0, 0)) {
        fprintf(stderr, "Error running script '%s': %s\n", script_path, lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    lua_close(L);
    return 0;
}