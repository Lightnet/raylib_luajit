local enet = require("enet")

if enet.initialize() ~= 0 then
    print("Failed to initialize ENet")
    return
end

local address = {host = "::", port = 12345, sin6_scope_id = 0}
local server = enet.host_create(address, 32, 2, 0, 0)
if not server then
    print("Failed to create ENet server")
    enet.deinitialize()
    return
end
print("Server started on [::]:12345")

while true do
    local event = enet.host_service(server, 1000)
    if type(event) == "table" then
        if event.type == enet.EVENT_TYPE_CONNECT then
            print("Client connected")
        elseif event.type == enet.EVENT_TYPE_RECEIVE then
            local data = enet.packet_data(event.packet)
            print("Received: " .. data)
            local packet = enet.packet_create(data, 1)  -- Reliable
            enet.peer_send(event.peer, 0, packet)
        elseif event.type == enet.EVENT_TYPE_DISCONNECT then
            print("Client disconnected")
        end
    end
end

enet.host_destroy(server)
enet.deinitialize()