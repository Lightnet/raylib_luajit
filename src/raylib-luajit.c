#include "raylib-luajit.h"
#include "luajit_utils.h"
#include <raylib.h>

// Core
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

static int l_SetTargetFPS(lua_State *L) {
    int fps = luaL_checkinteger(L, 1);
    SetTargetFPS(fps);
    return 0;
}

static int l_GetFPS(lua_State *L) {
    lua_pushinteger(L, GetFPS());
    return 1;
}

static int l_GetFrameTime(lua_State *L) {
    lua_pushnumber(L, GetFrameTime());
    return 1;
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

// Input - Keyboard
static int l_IsKeyPressed(lua_State *L) {
    int key = luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsKeyPressed(key));
    return 1;
}

static int l_IsKeyDown(lua_State *L) {
    int key = luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsKeyDown(key));
    return 1;
}

static int l_GetKeyPressed(lua_State *L) {
    lua_pushinteger(L, GetKeyPressed());
    return 1;
}

// Input - Mouse
static int l_IsMouseButtonPressed(lua_State *L) {
    int button = luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsMouseButtonPressed(button));
    return 1;
}

static int l_GetMousePosition(lua_State *L) {
    Vector2 pos = GetMousePosition();
    push_vector2(L, pos);
    return 1;
}

// Shapes
static int l_DrawPixel(lua_State *L) {
    int posX = luaL_checkinteger(L, 1);
    int posY = luaL_checkinteger(L, 2);
    Color color = unpack_color(L, 3);
    DrawPixel(posX, posY, color);
    return 0;
}

static int l_DrawLine(lua_State *L) {
    int startPosX = luaL_checkinteger(L, 1);
    int startPosY = luaL_checkinteger(L, 2);
    int endPosX = luaL_checkinteger(L, 3);
    int endPosY = luaL_checkinteger(L, 4);
    Color color = unpack_color(L, 5);
    DrawLine(startPosX, startPosY, endPosX, endPosY, color);
    return 0;
}

static int l_DrawCircle(lua_State *L) {
    int centerX = luaL_checkinteger(L, 1);
    int centerY = luaL_checkinteger(L, 2);
    float radius = (float)luaL_checknumber(L, 3);
    Color color = unpack_color(L, 4);
    DrawCircle(centerX, centerY, radius, color);
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

static int l_DrawRectangleRec(lua_State *L) {
    Rectangle rec = unpack_rectangle(L, 1);
    Color color = unpack_color(L, 2);
    DrawRectangleRec(rec, color);
    return 0;
}

// Text
static int l_DrawText(lua_State *L) {
    const char *text = luaL_checkstring(L, 1);
    int posX = luaL_checkinteger(L, 2);
    int posY = luaL_checkinteger(L, 3);
    int fontSize = luaL_checkinteger(L, 4);
    Color color = unpack_color(L, 5);
    DrawText(text, posX, posY, fontSize, color);
    return 0;
}

static int l_MeasureText(lua_State *L) {
    const char *text = luaL_checkstring(L, 1);
    int fontSize = luaL_checkinteger(L, 2);
    lua_pushinteger(L, MeasureText(text, fontSize));
    return 1;
}

static const luaL_Reg raylib_funcs[] = {
    {"InitWindow", l_InitWindow},
    {"WindowShouldClose", l_WindowShouldClose},
    {"CloseWindow", l_CloseWindow},
    {"SetTargetFPS", l_SetTargetFPS},
    {"GetFPS", l_GetFPS},
    {"GetFrameTime", l_GetFrameTime},
    {"BeginDrawing", l_BeginDrawing},
    {"EndDrawing", l_EndDrawing},
    {"ClearBackground", l_ClearBackground},
    {"IsKeyPressed", l_IsKeyPressed},
    {"IsKeyDown", l_IsKeyDown},
    {"GetKeyPressed", l_GetKeyPressed},
    {"IsMouseButtonPressed", l_IsMouseButtonPressed},
    {"GetMousePosition", l_GetMousePosition},
    {"DrawPixel", l_DrawPixel},
    {"DrawLine", l_DrawLine},
    {"DrawCircle", l_DrawCircle},
    {"DrawRectangle", l_DrawRectangle},
    {"DrawRectangleRec", l_DrawRectangleRec},
    {"DrawText", l_DrawText},
    {"MeasureText", l_MeasureText},
    {NULL, NULL}
};

int luaopen_raylib(lua_State *L) {
    lua_newtable(L);
    luaL_setfuncs(L, raylib_funcs, 0);

    push_color(L, RAYWHITE); lua_setfield(L, -2, "RAYWHITE");
    push_color(L, BLACK);    lua_setfield(L, -2, "BLACK");
    push_color(L, RED);      lua_setfield(L, -2, "RED");
    push_color(L, GREEN);    lua_setfield(L, -2, "GREEN");
    push_color(L, BLUE);     lua_setfield(L, -2, "BLUE");
    push_color(L, YELLOW);   lua_setfield(L, -2, "YELLOW");
    push_color(L, PURPLE);   lua_setfield(L, -2, "PURPLE");
    push_color(L, GRAY);     lua_setfield(L, -2, "GRAY");

    lua_pushinteger(L, KEY_SPACE); lua_setfield(L, -2, "KEY_SPACE");
    lua_pushinteger(L, KEY_ESCAPE); lua_setfield(L, -2, "KEY_ESCAPE");
    lua_pushinteger(L, KEY_ENTER); lua_setfield(L, -2, "KEY_ENTER");
    lua_pushinteger(L, KEY_UP); lua_setfield(L, -2, "KEY_UP");
    lua_pushinteger(L, KEY_DOWN); lua_setfield(L, -2, "KEY_DOWN");
    lua_pushinteger(L, KEY_LEFT); lua_setfield(L, -2, "KEY_LEFT");
    lua_pushinteger(L, KEY_RIGHT); lua_setfield(L, -2, "KEY_RIGHT");

    lua_pushinteger(L, MOUSE_BUTTON_LEFT); lua_setfield(L, -2, "MOUSE_BUTTON_LEFT");
    lua_pushinteger(L, MOUSE_BUTTON_RIGHT); lua_setfield(L, -2, "MOUSE_BUTTON_RIGHT");

    return 1;
}