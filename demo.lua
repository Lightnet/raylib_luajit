local rl = require("rl")
local enet = require("enet")

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

rl.InitWindow(800, 450, "raylib + ENet Wrapper Test")
local rect_x = 0
local speed = 100
local connected = false

while not rl.WindowShouldClose() do
    local dt = rl.GetFrameTime()

    if rl.IsKeyPressed(265) then speed = speed + 50 end  -- KEY_UP
    if rl.IsKeyPressed(264) then speed = speed - 50 end  -- KEY_DOWN
    speed = math.max(10, speed)

    rect_x = rect_x + speed * dt
    if rect_x > 800 then rect_x = 0 end

    if connected then
        local data = tostring(rect_x)
        local packet = enet.packet_create(data, 1)  -- Reliable
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
            rect_x = tonumber(data) or rect_x
            enet.packet_destroy(event.packet)  -- Explicitly destroy received packet
        elseif event.type == enet.EVENT_TYPE_DISCONNECT then
            print("Disconnected from server")
            connected = false
        end
    end

    rl.BeginDrawing()
    rl.ClearBackground(colors.RAYWHITE)
    rl.DrawRectangle(rect_x, 200, 100, 50, connected and colors.GREEN or colors.RED)
    rl.DrawText("rect_x: " .. math.floor(rect_x), 10, 10, 20, colors.BLACK)
    rl.DrawText("Speed: " .. speed .. " (Up/Down to adjust)", 10, 30, 20, colors.BLACK)
    rl.DrawText("Status: " .. (connected and "Connected" or "Disconnected"), 10, 50, 20, colors.BLACK)
    rl.EndDrawing()
end

enet.host_destroy(client)
enet.deinitialize()
rl.CloseWindow()