local rl = require("rl")
local rg = require("rg")

rl.InitWindow(800, 600, "raylib + raygui Test")
rl.SetTargetFPS(60)

local pos = {400, 300}
local speed = 200
local radius = 20
local show_gui = false
local custom_orange = {255, 165, 0, 255}
local check_active = false
local toggle_active = false
local slider_value = 50
local progress_value = 0.5
local spinner_value = 10
local text_input = "Hello"
local dropdown_active = 0
local dropdown_open = false
local list_scroll = 0
local list_active = 0
local color_pick = {255, 0, 0, 255}
local scroll_pos = {0, 0}

while not rl.WindowShouldClose() do
    local dt = rl.GetFrameTime()

    if rl.IsKeyDown(rl.KEY_RIGHT) then pos[1] = pos[1] + speed * dt end
    if rl.IsKeyDown(rl.KEY_LEFT) then pos[1] = pos[1] - speed * dt end
    if rl.IsKeyDown(rl.KEY_DOWN) then pos[2] = pos[2] + speed * dt end
    if rl.IsKeyDown(rl.KEY_UP) then pos[2] = pos[2] - speed * dt end

    if rl.IsMouseButtonPressed(rl.MOUSE_BUTTON_LEFT) then
        radius = radius + 5
    elseif rl.IsMouseButtonPressed(rl.MOUSE_BUTTON_RIGHT) then
        radius = math.max(5, radius - 5)
    end

    rl.BeginDrawing()
    rl.ClearBackground(rl.RAYWHITE)

    rl.DrawCircle(pos[1], pos[2], radius, rl.GREEN)
    rl.DrawText("Move with arrows, resize with mouse", 10, 10, 20, rl.BLACK)

    if rg.GuiButton({700, 10, 80, 30}, "Toggle GUI") then
        show_gui = not show_gui
    end

    if show_gui then
        local y = 50
        rg.GuiLabel({10, y, 100, 20}, "Controls Test")
        y = y + 30

        check_active = rg.GuiCheckBox({10, y, 20, 20}, "Check Me", check_active)
        y = y + 30

        toggle_active = rg.GuiToggle({10, y, 100, 20}, "Toggle", toggle_active)
        y = y + 30

        slider_value = rg.GuiSlider({10, y, 150, 20}, "Min", "Max", slider_value, 0, 100)
        y = y + 30

        progress_value = rg.GuiProgressBar({10, y, 150, 20}, nil, nil, progress_value, 0, 1)
        y = y + 30

        spinner_value = rg.GuiSpinner({10, y, 100, 20}, "Spin", spinner_value, 0, 100)
        y = y + 30

        local new_text, edit_mode = rg.GuiTextBox({10, y, 150, 20}, text_input, true)
        if edit_mode then text_input = new_text end
        y = y + 30

        if rg.GuiWindowBox({200, 50, 200, 300}, "Window") then
            -- Close action
        end
        rg.GuiGroupBox({210, 80, 180, 260}, "Group")

        local result, scroll, view = rg.GuiScrollPanel({420, 50, 200, 200}, "Scroll", {0, 0, 400, 400}, scroll_pos)
        scroll_pos = scroll
        y = y + 30

        local new_active, clicked = rg.GuiDropdownBox({70, y, 150, 30}, "One;Two;Three", dropdown_active, dropdown_open)
        --print("Dropdown - New Active:", new_active, "Clicked:", clicked, "Current:", dropdown_active, "Open:", dropdown_open)
        if clicked then
            dropdown_open = not dropdown_open
            dropdown_active = new_active
        end
        y = y + 100

        list_scroll, list_active = rg.GuiListView({10, y, 150, 60}, "Item1;Item2;Item3", list_scroll, list_active)
        y = y + 70

        color_pick = rg.GuiColorPicker({10, y, 100, 100}, "Pick", color_pick)
        y = y + 110

        if rg.GuiMessageBox({300, 200, 200, 100}, "Message", "Test message", "OK") == 1 then
            -- OK clicked
        end
    end

    rl.EndDrawing()
end

rl.CloseWindow()