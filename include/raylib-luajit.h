#ifndef RAYLIB_LUAJIT_H
#define RAYLIB_LUAJIT_H

#include <lua.h>
#include <lauxlib.h>
#include <raylib.h>
#include <raymath.h>

// Helper to convert uint32_t RGBA to Color struct
static Color uint_to_color(unsigned int rgba) {
    Color c;
    c.r = (rgba >> 24) & 0xFF;  // Red
    c.g = (rgba >> 16) & 0xFF;  // Green
    c.b = (rgba >> 8) & 0xFF;   // Blue
    c.a = rgba & 0xFF;          // Alpha
    return c;
}

// Raylib functions
static int l_InitWindow(lua_State *L) {
    int width = (int)luaL_checkinteger(L, 1);
    int height = (int)luaL_checkinteger(L, 2);
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
    unsigned int rgba = (unsigned int)luaL_checkinteger(L, 1);
    Color color = uint_to_color(rgba);
    ClearBackground(color);
    return 0;
}

static int l_DrawRectangle(lua_State *L) {
    int posX = (int)luaL_checkinteger(L, 1);
    int posY = (int)luaL_checkinteger(L, 2);
    int width = (int)luaL_checkinteger(L, 3);
    int height = (int)luaL_checkinteger(L, 4);
    unsigned int rgba = (unsigned int)luaL_checkinteger(L, 5);
    Color color = uint_to_color(rgba);
    DrawRectangle(posX, posY, width, height, color);
    return 0;
}

static int l_GetFrameTime(lua_State *L) {
    lua_pushnumber(L, GetFrameTime());
    return 1;
}

static int l_DrawText(lua_State *L) {
    const char *text = luaL_checkstring(L, 1);
    int posX = (int)luaL_checkinteger(L, 2);
    int posY = (int)luaL_checkinteger(L, 3);
    int fontSize = (int)luaL_checkinteger(L, 4);
    unsigned int rgba = (unsigned int)luaL_checkinteger(L, 5);
    Color color = uint_to_color(rgba);
    DrawText(text, posX, posY, fontSize, color);
    return 0;
}

static int l_IsKeyPressed(lua_State *L) {
    int key = (int)luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsKeyPressed(key));
    return 1;
}

static int l_GetMousePosition(lua_State *L) {
    Vector2 pos = GetMousePosition();
    lua_newtable(L);
    lua_pushnumber(L, pos.x); lua_setfield(L, -2, "x");
    lua_pushnumber(L, pos.y); lua_setfield(L, -2, "y");
    return 1;
}

static int l_DrawCircle(lua_State *L) {
    int centerX = (int)luaL_checkinteger(L, 1);
    int centerY = (int)luaL_checkinteger(L, 2);
    float radius = (float)luaL_checknumber(L, 3);
    unsigned int rgba = (unsigned int)luaL_checkinteger(L, 4);
    Color color = uint_to_color(rgba);
    DrawCircle(centerX, centerY, radius, color);
    return 0;
}

static int l_SetTargetFPS(lua_State *L) {
    int fps = (int)luaL_checkinteger(L, 1);
    SetTargetFPS(fps);
    return 0;
}

// Raymath functions
static int l_Vector2Add(lua_State *L) {
    Vector2 v1 = { (float)luaL_checknumber(L, 1), (float)luaL_checknumber(L, 2) };
    Vector2 v2 = { (float)luaL_checknumber(L, 3), (float)luaL_checknumber(L, 4) };
    Vector2 result = Vector2Add(v1, v2);
    lua_newtable(L);
    lua_pushnumber(L, result.x); lua_setfield(L, -2, "x");
    lua_pushnumber(L, result.y); lua_setfield(L, -2, "y");
    return 1;
}

static int l_Vector2Scale(lua_State *L) {
    Vector2 v = { (float)luaL_checknumber(L, 1), (float)luaL_checknumber(L, 2) };
    float scale = (float)luaL_checknumber(L, 3);
    Vector2 result = Vector2Scale(v, scale);
    lua_newtable(L);
    lua_pushnumber(L, result.x); lua_setfield(L, -2, "x");
    lua_pushnumber(L, result.y); lua_setfield(L, -2, "y");
    return 1;
}

static int l_Vector2Subtract(lua_State *L) {
    Vector2 v1 = { (float)luaL_checknumber(L, 1), (float)luaL_checknumber(L, 2) };
    Vector2 v2 = { (float)luaL_checknumber(L, 3), (float)luaL_checknumber(L, 4) };
    Vector2 result = Vector2Subtract(v1, v2);
    lua_newtable(L);
    lua_pushnumber(L, result.x); lua_setfield(L, -2, "x");
    lua_pushnumber(L, result.y); lua_setfield(L, -2, "y");
    return 1;
}

// Table of functions to register with Lua
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
    {"GetMousePosition", l_GetMousePosition},
    {"DrawCircle", l_DrawCircle},
    {"SetTargetFPS", l_SetTargetFPS},
    {"Vector2Add", l_Vector2Add},
    {"Vector2Scale", l_Vector2Scale},
    {"Vector2Subtract", l_Vector2Subtract},
    {NULL, NULL}
};

// Register raylib with Lua
int luaopen_raylib(lua_State *L) {
    luaL_newlib(L, raylib_funcs);
    // Add color constants (RGBA)
    lua_pushinteger(L, 0xF5F5F5FF); lua_setfield(L, -2, "RAYWHITE");
    lua_pushinteger(L, 0x000000FF); lua_setfield(L, -2, "BLACK");
    lua_pushinteger(L, 0xFF0000FF); lua_setfield(L, -2, "RED");
    lua_pushinteger(L, 0x00FF00FF); lua_setfield(L, -2, "GREEN");
    lua_pushinteger(L, 0x0000FFFF); lua_setfield(L, -2, "BLUE");
    // Add key constants
    lua_pushinteger(L, 32); lua_setfield(L, -2, "KEY_SPACE");
    lua_pushinteger(L, 265); lua_setfield(L, -2, "KEY_UP");
    lua_pushinteger(L, 264); lua_setfield(L, -2, "KEY_DOWN");
    return 1;
}

#endif