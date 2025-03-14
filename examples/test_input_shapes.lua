local rl = require("rl")
local rg = require("rg")

rl.InitWindow(800, 450, "raylib LuaJIT Test")
rl.SetTargetFPS(60)

local pos = {400, 225}  -- Center of screen
local speed = 200
local radius = 20
local show_gui = false
local custom_orange = {255, 165, 0, 255}  -- Custom color

while not rl.WindowShouldClose() do
    local dt = rl.GetFrameTime()

    -- Move with arrow keys
    if rl.IsKeyDown(rl.KEY_RIGHT) then pos[1] = pos[1] + speed * dt end
    if rl.IsKeyDown(rl.KEY_LEFT) then pos[1] = pos[1] - speed * dt end
    if rl.IsKeyDown(rl.KEY_DOWN) then pos[2] = pos[2] + speed * dt end
    if rl.IsKeyDown(rl.KEY_UP) then pos[2] = pos[2] - speed * dt end

    -- Resize with mouse click
    if rl.IsMouseButtonPressed(rl.MOUSE_BUTTON_LEFT) then
        radius = radius + 5
    elseif rl.IsMouseButtonPressed(rl.MOUSE_BUTTON_RIGHT) then
        radius = math.max(5, radius - 5)
    end

    rl.BeginDrawing()
    rl.ClearBackground(rl.RAYWHITE)

    -- Draw shapes
    rl.DrawPixel(50, 50, rl.RED)
    rl.DrawLine(100, 100, 200, 200, rl.BLUE)
    rl.DrawCircle(pos[1], pos[2], radius, rl.GREEN)
    rl.DrawRectangle(300, 150, 100, 50, custom_orange)
    rl.DrawRectangleRec({500, 150, 80, 80}, rl.PURPLE)

    -- Draw text
    rl.DrawText("Move with arrows, resize with mouse!", 10, 10, 20, rl.BLACK)
    rl.DrawText("FPS: " .. rl.GetFPS(), 10, 30, 20, rl.GRAY)
    local mouse_pos = rl.GetMousePosition()
    rl.DrawText("Mouse: " .. math.floor(mouse_pos[1]) .. ", " .. math.floor(mouse_pos[2]), 10, 50, 20, rl.BLACK)

    -- GUI
    if rg.GuiButton({700, 10, 80, 30}, "Toggle GUI") then
        show_gui = not show_gui
    end
    if show_gui then
        rg.GuiLabel({10, 80, 200, 20}, "Radius: " .. radius)
        if rg.GuiCheckBox({10, 110, 20, 20}, "Freeze", false) then
            speed = (speed == 0) and 200 or 0
        end
    end

    rl.EndDrawing()
end

rl.CloseWindow()