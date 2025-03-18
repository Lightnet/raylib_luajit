rl = require("rl")

rl.InitWindow(800, 600, "Raylib LuaJIT Input Test")
rl.SetTargetFPS(60)

-- Set custom exit key
rl.SetExitKey(rl.KEY_Q)  -- Quit with 'Q' instead of ESC

-- Variables
local rect_x = 300
local rect_y = 200
local text_input = ""

while not rl.WindowShouldClose() do
    rl.BeginDrawing()
    rl.ClearBackground(rl.RAYWHITE)

    -- Mouse input
    local mouse_pos = rl.GetMousePosition()
    local mouse_x = rl.GetMouseX()
    local mouse_y = rl.GetMouseY()
    local wheel = rl.GetMouseWheelMove()

    -- Draw mouse info
    rl.DrawText("Mouse Pos: " .. mouse_pos[1] .. ", " .. mouse_pos[2], 10, 10, 20, rl.BLACK)
    rl.DrawText("Mouse X: " .. mouse_x .. ", Y: " .. mouse_y, 10, 30, 20, rl.BLACK)
    rl.DrawText("Wheel Move: " .. wheel, 10, 50, 20, rl.BLACK)

    -- Mouse button states
    if rl.IsMouseButtonPressed(rl.MOUSE_BUTTON_LEFT) then
        rl.DrawText("Left Click Pressed!", 10, 70, 20, rl.RED)
    end
    if rl.IsMouseButtonDown(rl.MOUSE_BUTTON_LEFT) then
        rect_x = mouse_x
        rect_y = mouse_y
    end
    if rl.IsMouseButtonReleased(rl.MOUSE_BUTTON_LEFT) then
        rl.DrawText("Left Click Released!", 10, 90, 20, rl.GREEN)
    end

    -- Change cursor on right click
    if rl.IsMouseButtonPressed(rl.MOUSE_BUTTON_RIGHT) then
        rl.SetMouseCursor(rl.MOUSE_CURSOR_POINTING_HAND)
    end
    if rl.IsMouseButtonReleased(rl.MOUSE_BUTTON_RIGHT) then
        rl.SetMouseCursor(rl.MOUSE_CURSOR_DEFAULT)
    end

    -- Keyboard input
    if rl.IsKeyPressed(rl.KEY_SPACE) then
        rl.DrawText("Space Pressed!", 10, 110, 20, rl.BLUE)
    end
    if rl.IsKeyDown(rl.KEY_LEFT) then rect_x = rect_x - 5 end
    if rl.IsKeyDown(rl.KEY_RIGHT) then rect_x = rect_x + 5 end
    if rl.IsKeyDown(rl.KEY_UP) then rect_y = rect_y - 5 end
    if rl.IsKeyDown(rl.KEY_DOWN) then rect_y = rect_y + 5 end
    if rl.IsKeyReleased(rl.KEY_SPACE) then
        rl.DrawText("Space Released!", 10, 130, 20, rl.PURPLE)
    end

    -- Text input with GetCharPressed
    local char = rl.GetCharPressed()
    while char ~= nil do
        if char >= 32 and char <= 126 then  -- Printable ASCII range
            text_input = text_input .. string.char(char)
        end
        char = rl.GetCharPressed()
    end
    if rl.IsKeyPressed(rl.KEY_BACKSPACE) and #text_input > 0 then
        text_input = text_input:sub(1, -2)
    end

    -- Draw rectangle and text
    rl.DrawRectangle(rect_x, rect_y, 50, 50, rl.BLUE)
    rl.DrawText("Typed: " .. text_input, 10, 150, 20, rl.BLACK)
    rl.DrawText("Move with arrows, Q to quit, Right-click for cursor change", 10, 170, 20, rl.GRAY)

    rl.EndDrawing()
end

rl.CloseWindow()