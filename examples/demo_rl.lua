-- TEST BUILD OUTDATE BUT REFS

local bit = require("bit")

rl.InitWindow(800, 450, "raylib [core] demo - LuaJIT")
rl.SetTargetFPS(60)
local rect_x = 0
local rect_y = 200
local speed = 200
local mouse_follow = false

while not rl.WindowShouldClose() do
    local dt = rl.GetFrameTime()
    
    if rl.IsKeyPressed(rl.KEY_SPACE) then
        mouse_follow = not mouse_follow
    end

    if mouse_follow then
        local mouse = rl.GetMousePosition()
        rect_x = mouse.x - 50
        rect_y = mouse.y - 25
    else
        rect_x = rect_x + speed * dt
        if rect_x > 800 then rect_x = -100 end
    end

    if rl.IsKeyPressed(rl.KEY_UP) then speed = speed + 50 end
    if rl.IsKeyPressed(rl.KEY_DOWN) then speed = speed - 50 end

    local color = bit.bor(
        bit.lshift(bit.band(math.floor(rect_x / 3) % 256, 0xFF), 16),
        bit.lshift(bit.band(math.floor(rect_x / 5) % 256, 0xFF), 8),
        bit.band(math.floor(rect_x / 7) % 256, 0xFF),
        0xFF
    )

    local offset = rl.Vector2Add(rect_x, rect_y, 20, 30)
    local scaled_offset = rl.Vector2Scale(offset.x, offset.y, 0.5)
    local circle_pos = rl.Vector2Subtract(rect_x, rect_y, 50, 50)

    rl.BeginDrawing()
    rl.ClearBackground(rl.RAYWHITE)
    rl.DrawRectangle(rect_x, rect_y, 100, 50, color)
    rl.DrawRectangle(scaled_offset.x, scaled_offset.y, 50, 25, rl.GREEN)
    rl.DrawCircle(circle_pos.x, circle_pos.y, 20, rl.BLUE)
    rl.DrawText("FPS: " .. math.floor(1 / dt), 10, 10, 20, rl.BLACK)
    rl.DrawText("Speed: " .. speed, 10, 30, 20, rl.BLACK)
    rl.DrawText("Press SPACE to toggle mouse follow", 10, 50, 20, rl.BLUE)
    rl.EndDrawing()
end

rl.CloseWindow()