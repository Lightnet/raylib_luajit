local flecs = require "flecs"
local ffi = require "ffi"

ffi.cdef [[
    typedef float ecs_f32_t;
    typedef struct { ecs_f32_t x, y; } Position;
    typedef struct { ecs_f32_t x, y; } Velocity;
]]

-- Function to list types
local function list_types()
    local test_values = {
        ["nil"] = nil,
        ["boolean"] = true,
        ["number"] = 42,
        ["string"] = "hello",
        ["table"] = {},
        ["function"] = function() end,
        ["userdata"] = world,  -- From flecs.ecs_init()
        ["cdata"] = ffi.new("Position"),
        ["casted_cdata"] = ffi.cast("void*", ffi.new("Position")),
    }
    print("Listing Lua types:")
    for name, value in pairs(test_values) do
        print(string.format("  %-12s: %s", name, type(value)))
    end
end

print("init...")
local world = flecs.ecs_init()
list_types()

flecs.ecs_component_init(world, "Position", ffi.sizeof("Position"))
flecs.ecs_component_init(world, "Velocity", ffi.sizeof("Velocity"))

local e = flecs.ecs_new_low_id(world)

local pos = ffi.new("Position")
pos.x = 10
pos.y = 20
local casted_pos = ffi.cast("void*", pos)
print("DEBUG: Type of pos = " .. type(pos) .. ", Type of casted_pos = " .. type(casted_pos))
flecs.ecs_set_id(world, e, "Position", casted_pos)

local vel = ffi.new("Velocity")
vel.x = 1
vel.y = 2
flecs.ecs_set_id(world, e, "Velocity", ffi.cast("void*", vel))

local pos_ptr = flecs.ecs_get_id(world, e, "Position")
if pos_ptr then
    local pos_data = ffi.cast("Position*", pos_ptr)
    print("Position: x=" .. pos_data.x .. ", y=" .. pos_data.y)
end

local vel_ptr = flecs.ecs_get_id(world, e, "Velocity")
if vel_ptr then
    local vel_data = ffi.cast("Velocity*", vel_ptr)
    print("Velocity: x=" .. vel_data.x .. ", y=" .. vel_data.y)
end

flecs.ecs_fini(world)