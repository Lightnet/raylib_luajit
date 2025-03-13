local ffi = require("ffi")

ffi.cdef[[
    typedef struct Color {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } Color;

    void InitWindow(int width, int height, const char *title);
    bool WindowShouldClose(void);
    void CloseWindow(void);
    void BeginDrawing(void);
    void EndDrawing(void);
    void ClearBackground(Color color);
    void DrawRectangle(int posX, int posY, int width, int height, Color color);
    float GetFrameTime(void);
    void DrawText(const char *text, int posX, int posY, int fontSize, Color color);
    bool IsKeyPressed(int key);
    typedef struct Vector2 { float x; float y; } Vector2;
    Vector2 GetMousePosition(void);
    void DrawCircle(int centerX, int centerY, float radius, Color color);
    void SetTargetFPS(int fps);
    Vector2 Vector2Add(Vector2 v1, Vector2 v2);
    Vector2 Vector2Scale(Vector2 v, float scale);
    Vector2 Vector2Subtract(Vector2 v1, Vector2 v2);
]]

local rlffi = ffi.load("raylib")
local bit = require("bit")

-- Helper to convert RGBA uint32_t to Color struct
local function to_color(rgba)
    return ffi.new("Color", {
        r = bit.rshift(rgba, 24),
        g = bit.band(bit.rshift(rgba, 16), 0xFF),
        b = bit.band(bit.rshift(rgba, 8), 0xFF),
        a = bit.band(rgba, 0xFF)
    })
end

rlffi.InitWindow(800, 450, "raylib [core] demo - LuaJIT FFI")
rlffi.SetTargetFPS(60)
local rect_x = 0
local rect_y = 200
local speed = 200
local mouse_follow = false

while not rlffi.WindowShouldClose() do
    local dt = rlffi.GetFrameTime()
    --print("Hello")
    
    if rlffi.IsKeyPressed(32) then  -- KEY_SPACE
        mouse_follow = not mouse_follow
    end

    if mouse_follow then
        local mouse = rlffi.GetMousePosition()
        rect_x = mouse.x - 50
        rect_y = mouse.y - 25
    else
        rect_x = rect_x + speed * dt
        if rect_x > 800 then rect_x = -100 end
    end

    if rlffi.IsKeyPressed(265) then speed = speed + 50 end  -- KEY_UP
    if rlffi.IsKeyPressed(264) then speed = speed - 50 end  -- KEY_DOWN

    -- Print rect_x to verify movement
    --print("rect_x: " .. rect_x)

    local color = bit.bor(
        bit.lshift(bit.band(math.floor(rect_x / 3) % 256, 0xFF), 16),
        bit.lshift(bit.band(math.floor(rect_x / 5) % 256, 0xFF), 8),
        bit.band(math.floor(rect_x / 7) % 256, 0xFF),
        0xFF
    )

    local offset = rlffi.Vector2Add({x = rect_x, y = rect_y}, {x = 20, y = 30})
    local scaled_offset = rlffi.Vector2Scale(offset, 0.5)
    local circle_pos = rlffi.Vector2Subtract({x = rect_x, y = rect_y}, {x = 50, y = 50})

    rlffi.BeginDrawing()
    rlffi.ClearBackground(to_color(0xF5F5F5FF))  -- RAYWHITE
    rlffi.DrawRectangle(rect_x, rect_y, 100, 50, to_color(color))
    rlffi.DrawRectangle(scaled_offset.x, scaled_offset.y, 50, 25, to_color(0x00FF00FF))  -- GREEN
    rlffi.DrawCircle(circle_pos.x, circle_pos.y, 20, to_color(0x0000FFFF))  -- BLUE
    rlffi.DrawText("FPS: " .. math.floor(1 / dt), 10, 10, 20, to_color(0x000000FF))  -- BLACK
    rlffi.DrawText("Speed: " .. speed, 10, 30, 20, to_color(0x000000FF))
    rlffi.DrawText("Press SPACE to toggle mouse follow", 10, 50, 20, to_color(0x0000FFFF))
    rlffi.EndDrawing()
end

rlffi.CloseWindow()