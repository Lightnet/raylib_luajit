#include "raylib-luajit.h"
#include <raylib.h>
#include <lauxlib.h>

// Helper to unpack a Color from a Lua table {r, g, b, a}
static Color unpack_color(lua_State *L, int idx) {
    Color c = {0, 0, 0, 255};  // Default to black, opaque
    if (lua_istable(L, idx)) {
        lua_rawgeti(L, idx, 1); c.r = lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, idx, 2); c.g = lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, idx, 3); c.b = lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, idx, 4); c.a = lua_isnil(L, -1) ? 255 : lua_tointeger(L, -1); lua_pop(L, 1);
    }
    return c;
}

static int l_InitWindow(lua_State *L) {
    int width = luaL_checkinteger(L, 1);
    int height = luaL_checkinteger(L, 2);
    const char *title = luaL_checkstring(L, 3);
    InitWindow(width, height, title);
    return 0;
}

static int l_WindowShouldClose(lua_State *L) {
    lua_pushboolean(L, WindowShouldClose());
    return 1;
}

static int l_CloseWindow(lua_State *L) {
    CloseWindow();
    return 0;
}

static int l_BeginDrawing(lua_State *L) {
    BeginDrawing();
    return 0;
}

static int l_EndDrawing(lua_State *L) {
    EndDrawing();
    return 0;
}

static int l_ClearBackground(lua_State *L) {
    Color color = unpack_color(L, 1);
    ClearBackground(color);
    return 0;
}

static int l_DrawRectangle(lua_State *L) {
    int posX = luaL_checkinteger(L, 1);
    int posY = luaL_checkinteger(L, 2);
    int width = luaL_checkinteger(L, 3);
    int height = luaL_checkinteger(L, 4);
    Color color = unpack_color(L, 5);
    DrawRectangle(posX, posY, width, height, color);
    return 0;
}

static int l_GetFrameTime(lua_State *L) {
    lua_pushnumber(L, GetFrameTime());
    return 1;
}

static int l_DrawText(lua_State *L) {
    const char *text = luaL_checkstring(L, 1);
    int posX = luaL_checkinteger(L, 2);
    int posY = luaL_checkinteger(L, 3);
    int fontSize = luaL_checkinteger(L, 4);
    Color color = unpack_color(L, 5);
    DrawText(text, posX, posY, fontSize, color);
    return 0;
}

static int l_IsKeyPressed(lua_State *L) {
    int key = luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsKeyPressed(key));
    return 1;
}

static const luaL_Reg raylib_funcs[] = {
    {"InitWindow", l_InitWindow},
    {"WindowShouldClose", l_WindowShouldClose},
    {"CloseWindow", l_CloseWindow},
    {"BeginDrawing", l_BeginDrawing},
    {"EndDrawing", l_EndDrawing},
    {"ClearBackground", l_ClearBackground},
    {"DrawRectangle", l_DrawRectangle},
    {"GetFrameTime", l_GetFrameTime},
    {"DrawText", l_DrawText},
    {"IsKeyPressed", l_IsKeyPressed},
    {NULL, NULL}
};

int luaopen_raylib(lua_State *L) {
    lua_newtable(L);
    luaL_setfuncs(L, raylib_funcs, 0);
    return 1;
}