#include "raygui-luajit.h"
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <lauxlib.h>

// Helper to unpack Rectangle from Lua table {x, y, width, height}
static Rectangle unpack_rectangle(lua_State *L, int idx) {
    Rectangle r = {0, 0, 0, 0};
    if (lua_istable(L, idx)) {
        lua_rawgeti(L, idx, 1); r.x = (float)lua_tonumber(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, idx, 2); r.y = (float)lua_tonumber(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, idx, 3); r.width = (float)lua_tonumber(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, idx, 4); r.height = (float)lua_tonumber(L, -1); lua_pop(L, 1);
    }
    return r;
}

// rg.GuiButton(bounds, text)
static int l_GuiButton(lua_State *L) {
    Rectangle bounds = unpack_rectangle(L, 1);
    const char *text = luaL_checkstring(L, 2);
    int clicked = GuiButton(bounds, text);
    lua_pushboolean(L, clicked);
    return 1;
}

// rg.GuiLabel(bounds, text)
static int l_GuiLabel(lua_State *L) {
    Rectangle bounds = unpack_rectangle(L, 1);
    const char *text = luaL_checkstring(L, 2);
    GuiLabel(bounds, text);
    return 0;
}

// rg.GuiCheckBox(bounds, text, checked)
static int l_GuiCheckBox(lua_State *L) {
    Rectangle bounds = unpack_rectangle(L, 1);
    const char *text = luaL_checkstring(L, 2);
    bool checked = lua_toboolean(L, 3);
    int result = GuiCheckBox(bounds, text, &checked);
    lua_pushboolean(L, checked);
    return 0;
}

static const luaL_Reg raygui_funcs[] = {
    {"GuiButton", l_GuiButton},
    {"GuiLabel", l_GuiLabel},
    {"GuiCheckBox", l_GuiCheckBox},
    {NULL, NULL}
};

int luaopen_raygui(lua_State *L) {
    lua_newtable(L);
    luaL_setfuncs(L, raygui_funcs, 0);
    return 1;
}