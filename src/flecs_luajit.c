#include "flecs_luajit.h"
#include <flecs.h>
#include <lauxlib.h>
//#include <luajit.h>
#include <stdio.h>

#define FLECS_WORLD_MT "FlecsWorld"
#define FLECS_ENTITY_MT "FlecsEntity"

// In flecs-luajit.c
// static int l_push_pointer(lua_State *L) {
//   void *ptr = ffi_cast(void*, lua_tocdata(L, 1));
//   lua_pushlightuserdata(L, ptr);
//   return 1;
// }

static ecs_world_t** push_flecs_world(lua_State *L) {
    ecs_world_t **world = (ecs_world_t**)lua_newuserdata(L, sizeof(ecs_world_t*));
    luaL_getmetatable(L, FLECS_WORLD_MT);
    lua_setmetatable(L, -2);
    return world;
}

static ecs_entity_t* push_flecs_entity(lua_State *L) {
    ecs_entity_t *entity = (ecs_entity_t*)lua_newuserdata(L, sizeof(ecs_entity_t));
    luaL_getmetatable(L, FLECS_ENTITY_MT);
    lua_setmetatable(L, -2);
    return entity;
}

// flecs.ecs_init()
static int l_flecs_ecs_init(lua_State *L) {
    ecs_world_t *world = ecs_init();
    if (!world) {
        lua_pushnil(L);
    } else {
        ecs_world_t **ud = push_flecs_world(L);
        *ud = world;
    }
    return 1;
}

// flecs.ecs_fini(world)
static int l_flecs_ecs_fini(lua_State *L) {
    ecs_world_t **world = (ecs_world_t**)luaL_checkudata(L, 1, FLECS_WORLD_MT);
    if (*world) {
        ecs_fini(*world);
        *world = NULL;
    }
    return 0;
}

// flecs.ecs_new_low_id(world)
static int l_flecs_ecs_new_low_id(lua_State *L) {
    ecs_world_t **world = (ecs_world_t**)luaL_checkudata(L, 1, FLECS_WORLD_MT);
    ecs_entity_t e = ecs_new_low_id(*world);
    ecs_entity_t *ud = push_flecs_entity(L);
    *ud = e;
    return 1;
}

// flecs.ecs_new_w_id(world, id_name)
static int l_flecs_ecs_new(lua_State *L) {
  ecs_world_t **world = (ecs_world_t**)luaL_checkudata(L, 1, FLECS_WORLD_MT);
  ecs_entity_t e = ecs_new(*world);
  ecs_entity_t *ud = push_flecs_entity(L);
  *ud = e;
  return 1;
}

// flecs.ecs_new_w_id(world, id_name)
static int l_flecs_ecs_new_w_id(lua_State *L) {
    ecs_world_t **world = (ecs_world_t**)luaL_checkudata(L, 1, FLECS_WORLD_MT);
    const char *id_name = luaL_checkstring(L, 2);
    ecs_entity_t id = ecs_lookup(*world, id_name);
    if (id == 0) {
        luaL_error(L, "ID '%s' not found", id_name);
    }
    ecs_entity_t e = ecs_new_w_id(*world, id);
    ecs_entity_t *ud = push_flecs_entity(L);
    *ud = e;
    return 1;
}


// Helper to register components
static int l_flecs_ecs_component_init(lua_State *L) {
  ecs_world_t **world = (ecs_world_t**)luaL_checkudata(L, 1, FLECS_WORLD_MT);
  const char *name = luaL_checkstring(L, 2);
  size_t size = (size_t)luaL_checkinteger(L, 3);
  printf("ecs_component_init: name='%s', size=%zu, sizeof(ecs_f32_t)=%zu\n", name, size, sizeof(ecs_f32_t));

  ecs_entity_desc_t desc = {0};
  desc.name = name;
  ecs_entity_t component = ecs_entity_init(*world, &desc);
  ecs_set(*world, component, EcsComponent, {size, sizeof(void*)});

  printf("ecs_component_init: registered component '%s' with entity=%llu\n", name, (unsigned long long)component);
  return 0;
}


// flecs.ecs_add_id(world, entity, id_name)
static int l_flecs_ecs_add_id(lua_State *L) {
    ecs_world_t **world = (ecs_world_t**)luaL_checkudata(L, 1, FLECS_WORLD_MT);
    ecs_entity_t *entity = (ecs_entity_t*)luaL_checkudata(L, 2, FLECS_ENTITY_MT);
    const char *id_name = luaL_checkstring(L, 3);
    ecs_entity_t id = ecs_lookup(*world, id_name);
    if (id == 0) {
        id = ecs_entity_init(*world, &(ecs_entity_desc_t){.name = id_name});
    }
    ecs_add_id(*world, *entity, id);
    return 0;
}

// flecs.ecs_remove_id(world, entity, id_name)
static int l_flecs_ecs_remove_id(lua_State *L) {
    ecs_world_t **world = (ecs_world_t**)luaL_checkudata(L, 1, FLECS_WORLD_MT);
    ecs_entity_t *entity = (ecs_entity_t*)luaL_checkudata(L, 2, FLECS_ENTITY_MT);
    const char *id_name = luaL_checkstring(L, 3);
    ecs_entity_t id = ecs_lookup(*world, id_name);
    if (id == 0) {
        luaL_error(L, "ID '%s' not found", id_name);
    }
    ecs_remove_id(*world, *entity, id);
    return 0;
}

// flecs.ecs_has_id(world, entity, id_name)
static int l_flecs_ecs_has_id(lua_State *L) {
    ecs_world_t **world = (ecs_world_t**)luaL_checkudata(L, 1, FLECS_WORLD_MT);
    ecs_entity_t *entity = (ecs_entity_t*)luaL_checkudata(L, 2, FLECS_ENTITY_MT);
    const char *id_name = luaL_checkstring(L, 3);
    ecs_entity_t id = ecs_lookup(*world, id_name);
    if (id == 0) {
        lua_pushboolean(L, false);
    } else {
        lua_pushboolean(L, ecs_has_id(*world, *entity, id));
    }
    return 1;
}


static int l_flecs_ecs_set_id(lua_State *L) {
  ecs_world_t **world = (ecs_world_t**)luaL_checkudata(L, 1, FLECS_WORLD_MT);
  ecs_entity_t *entity = (ecs_entity_t*)luaL_checkudata(L, 2, FLECS_ENTITY_MT);
  const char *component_name = luaL_checkstring(L, 3);
  printf("ecs_set_id: entering, component='%s', stack top=%d\n", component_name, lua_gettop(L));

  ecs_entity_t component = ecs_lookup(*world, component_name);
  if (component == 0) {
      luaL_error(L, "Component '%s' not found; register it with ecs_component_init", component_name);
  }

  const ecs_type_info_t *ti = ecs_get_type_info(*world, component);
  if (!ti || ti->size == 0) {
      luaL_error(L, "Component '%s' has no size info", component_name);
  }

  int type = lua_type(L, 4);
  const char *type_name = lua_typename(L, type);
  printf("ecs_set_id: component='%s', arg type=%d (%s), is_lightuserdata=%d, is_userdata=%d\n", 
         component_name, type, type_name, lua_islightuserdata(L, 4), lua_isuserdata(L, 4));

  void *data = NULL;
  if (lua_islightuserdata(L, 4)) {
      data = lua_touserdata(L, 4);
      printf("ecs_set_id: lightuserdata detected, data=%p\n", data);
  } else if (type == 10 || lua_isuserdata(L, 4)) {  // Explicitly catch type 10
      void **udata = (void**)lua_touserdata(L, 4);
      printf("ecs_set_id: attempting to extract pointer from type=%d, udata=%p\n", type, udata);
      if (udata) {
          data = *udata;
          printf("ecs_set_id: userdata (cdata) detected, extracted data=%p\n", data);
      } else {
          luaL_error(L, "Invalid cdata pointer (null udata) for component '%s'", component_name);
      }
  } else {
      luaL_error(L, "Expected lightuserdata or userdata (cdata) for component '%s', got %s", 
                 component_name, type_name);
  }

  if (data) {
      printf("ecs_set_id: calling ecs_set_id with size=%zu\n", ti->size);
      ecs_set_id(*world, *entity, component, ti->size, data);
      printf("ecs_set_id: set component '%s' successfully\n", component_name);
  } else {
      luaL_error(L, "Failed to extract pointer for component '%s'", component_name);
  }
  printf("ecs_set_id: exiting\n");
  return 0;
}

// flecs.ecs_get_id(world, entity, component_name)
static int l_flecs_ecs_get_id(lua_State *L) {
    ecs_world_t **world = (ecs_world_t**)luaL_checkudata(L, 1, FLECS_WORLD_MT);
    ecs_entity_t *entity = (ecs_entity_t*)luaL_checkudata(L, 2, FLECS_ENTITY_MT);
    const char *component_name = luaL_checkstring(L, 3);
    ecs_entity_t component = ecs_lookup(*world, component_name);
    if (component == 0) {
        lua_pushnil(L);
        return 1;
    }

    const void *data = ecs_get_id(*world, *entity, component);
    if (data) {
        lua_pushlightuserdata(L, (void*)data);
    } else {
        lua_pushnil(L);
    }
    return 1;
}

// flecs.ecs_progress(world, [delta_time])
static int l_flecs_ecs_progress(lua_State *L) {
    ecs_world_t **world = (ecs_world_t**)luaL_checkudata(L, 1, FLECS_WORLD_MT);
    float delta_time = (float)luaL_optnumber(L, 2, 0.0f);
    ecs_progress(*world, delta_time);
    return 0;
}

// Garbage collection for world
static int flecs_world_gc(lua_State *L) {
    ecs_world_t **world = (ecs_world_t**)luaL_checkudata(L, 1, FLECS_WORLD_MT);
    if (*world) {
        ecs_fini(*world);
        *world = NULL;
    }
    return 0;
}

static const luaL_Reg flecs_funcs[] = {
    {"ecs_init", l_flecs_ecs_init},
    {"ecs_fini", l_flecs_ecs_fini},
    {"ecs_new_low_id", l_flecs_ecs_new_low_id},
    {"ecs_new", l_flecs_ecs_new},
    {"ecs_new_w_id", l_flecs_ecs_new_w_id},
    {"ecs_component_init", l_flecs_ecs_component_init},
    {"ecs_add_id", l_flecs_ecs_add_id},
    {"ecs_remove_id", l_flecs_ecs_remove_id},
    {"ecs_has_id", l_flecs_ecs_has_id},
    {"ecs_set_id", l_flecs_ecs_set_id},
    {"ecs_get_id", l_flecs_ecs_get_id},
    {"ecs_progress", l_flecs_ecs_progress},
    //{"push_pointer", l_push_pointer},
    {NULL, NULL}
};

static const luaL_Reg flecs_world_mt[] = {
    {"__gc", flecs_world_gc},
    {NULL, NULL}
};

static const luaL_Reg flecs_entity_mt[] = {
    {NULL, NULL}
};

int luaopen_flecs(lua_State *L) {
    luaL_newmetatable(L, FLECS_WORLD_MT);
    luaL_setfuncs(L, flecs_world_mt, 0);
    lua_pop(L, 1);

    luaL_newmetatable(L, FLECS_ENTITY_MT);
    luaL_setfuncs(L, flecs_entity_mt, 0);
    lua_pop(L, 1);

    lua_newtable(L);
    luaL_setfuncs(L, flecs_funcs, 0);
    return 1;
}