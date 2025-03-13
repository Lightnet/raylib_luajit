local ffi = require("ffi")

ffi.cdef[[
    typedef struct Color {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } Color;

    typedef struct Vector2 {
        float x;
        float y;
    } Vector2;

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
    Vector2 GetMousePosition(void);
    void DrawCircle(int centerX, int centerY, float radius, Color color);
    void SetTargetFPS(int fps);
    Vector2 Vector2Add(Vector2 v1, Vector2 v2);
    Vector2 Vector2Scale(Vector2 v, float scale);
    Vector2 Vector2Subtract(Vector2 v1, Vector2 v2);
]]

local rl_ffi = ffi.load("raylib")
local bit = require("bit")

-- Raylib color constants (RGBA format, matching raylib.h)
local colors = {
  RAYWHITE = ffi.new("Color", 245, 245, 245, 255),
  BLACK    = ffi.new("Color", 0, 0, 0, 255),
  RED      = ffi.new("Color", 255, 0, 0, 255),
  GREEN    = ffi.new("Color", 0, 255, 0, 255),
  BLUE     = ffi.new("Color", 0, 0, 255, 255),
  YELLOW   = ffi.new("Color", 255, 255, 0, 255),
  PURPLE   = ffi.new("Color", 130, 0, 130, 255),
  GRAY     = ffi.new("Color", 130, 130, 130, 255)
}

-- Helper to create a Color from RGBA integers (for dynamic colors)
local function rgba(r, g, b, a)
    return ffi.new("Color", r, g, b, a)
end

rl_ffi.InitWindow(800, 450, "raylib [core] demo - LuaJIT FFI")
rl_ffi.SetTargetFPS(60)
local rect_x = 0
local rect_y = 200
local speed = 200
local mouse_follow = false

while not rl_ffi.WindowShouldClose() do
    local dt = rl_ffi.GetFrameTime()
    
    if rl_ffi.IsKeyPressed(32) then  -- KEY_SPACE
        mouse_follow = not mouse_follow
    end

    if mouse_follow then
        local mouse = rl_ffi.GetMousePosition()
        rect_x = mouse.x - 50
        rect_y = mouse.y - 25
    else
        rect_x = rect_x + speed * dt
        if rect_x > 800 then rect_x = -100 end
    end

    if rl_ffi.IsKeyPressed(265) then speed = speed + 50 end  -- KEY_UP
    if rl_ffi.IsKeyPressed(264) then speed = speed - 50 end  -- KEY_DOWN

    -- Dynamic color using rgba helper
    local color = rgba(
        math.floor(rect_x / 3) % 256,
        math.floor(rect_x / 5) % 256,
        math.floor(rect_x / 7) % 256,
        255
    )

    local offset = rl_ffi.Vector2Add({x = rect_x, y = rect_y}, {x = 20, y = 30})
    local scaled_offset = rl_ffi.Vector2Scale(offset, 0.5)
    local circle_pos = rl_ffi.Vector2Subtract({x = rect_x, y = rect_y}, {x = 50, y = 50})

    rl_ffi.BeginDrawing()
    rl_ffi.ClearBackground(colors.RAYWHITE)
    rl_ffi.DrawRectangle(rect_x, rect_y, 100, 50, color)
    rl_ffi.DrawRectangle(scaled_offset.x, scaled_offset.y, 50, 25, colors.GREEN)
    rl_ffi.DrawCircle(circle_pos.x, circle_pos.y, 20, colors.BLUE)
    rl_ffi.DrawText("FPS: " .. math.floor(1 / dt), 10, 10, 20, colors.BLACK)
    rl_ffi.DrawText("Speed: " .. speed, 10, 30, 20, colors.BLACK)
    rl_ffi.DrawText("Press SPACE to toggle mouse follow", 10, 50, 20, colors.BLUE)
    rl_ffi.EndDrawing()
end

rl_ffi.CloseWindow()