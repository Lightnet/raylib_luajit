local rl = require("rl")
local enet = require("enet")
local rm = require("rm")
local rg = require("rg")

local colors = {
    RAYWHITE = {245, 245, 245, 255},
    BLACK    = {0, 0, 0, 255},
    RED      = {255, 0, 0, 255},
    GREEN    = {0, 255, 0, 255},
}

if enet.initialize() ~= 0 then
    print("Failed to initialize ENet")
    return
end

local client = enet.host_create(nil, 1, 2, 0, 0)
if not client then
    print("Failed to create ENet client")
    enet.deinitialize()
    return
end

local address = {host = "127.0.0.1", port = 12345, sin6_scope_id = 0}
local peer = enet.host_connect(client, address, 2, 0)
if not peer then
    print("No available peers")
    enet.host_destroy(client)
    enet.deinitialize()
    return
end

rl.InitWindow(800, 450, "raylib + ENet + Raymath + Raygui Test")
local pos = {0, 200, 0}  -- Vector3 as table {x, y, z}
local speed = 100
local connected = false
local show_gui = false

while not rl.WindowShouldClose() do
    local dt = rl.GetFrameTime()

    if rl.IsKeyPressed(265) then speed = speed + 50 end  -- KEY_UP
    if rl.IsKeyPressed(264) then speed = speed - 50 end  -- KEY_DOWN
    speed = math.max(10, speed)

    -- Move using Vector3Add
    local velocity = {speed * dt, 0, 0}
    pos = rm.Vector3Add(pos, velocity)
    if pos[1] > 800 then pos[1] = 0 end

    if connected then
        local data = tostring(pos[1])
        local packet = enet.packet_create(data, 1)
        local result = enet.peer_send(peer, 0, packet)
        if result ~= 0 then
            print("Failed to send packet: " .. result)
        end
    end

    local event = enet.host_service(client, 0)
    if type(event) == "table" then
        if event.type == enet.EVENT_TYPE_CONNECT then
            print("Connected to server")
            connected = true
        elseif event.type == enet.EVENT_TYPE_RECEIVE then
            local data = enet.packet_data(event.packet)
            pos[1] = tonumber(data) or pos[1]
            enet.packet_destroy(event.packet)
        elseif event.type == enet.EVENT_TYPE_DISCONNECT then
            print("Disconnected from server")
            connected = false
        end
    end

    rl.BeginDrawing()
    rl.ClearBackground(colors.RAYWHITE)
    rl.DrawRectangle(pos[1], pos[2], 100, 50, connected and colors.GREEN or colors.RED)
    rl.DrawText("pos.x: " .. math.floor(pos[1]), 10, 10, 20, colors.BLACK)
    rl.DrawText("Speed: " .. speed .. " (Up/Down to adjust)", 10, 30, 20, colors.BLACK)
    rl.DrawText("Status: " .. (connected and "Connected" or "Disconnected"), 10, 50, 20, colors.BLACK)

    -- Raygui test
    if rg.GuiButton({700, 10, 80, 30}, "Toggle GUI") then
        show_gui = not show_gui
    end
    if show_gui then
        rg.GuiLabel({10, 80, 100, 20}, "Test Label")
        if rg.GuiCheckBox({10, 110, 20, 20}, "Connected?", connected) then
            connected = not connected  -- Toggle for demo
        end
    end

    rl.EndDrawing()
end

enet.host_destroy(client)
enet.deinitialize()
rl.CloseWindow()