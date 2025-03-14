Grok Format this cheetsheets. (Draft)

Custom Colors in raylib-luajit

In our current raylib-luajit.c, colors are handled via tables {r, g, b, a} because unpack_color expects this format. This matches raylib’s Color struct (unsigned char r, g, b, a), where each component is an integer from 0 to 255.

Format

- Lua: {r, g, b, a}
    - r: Red (0-255)
    - g: Green (0-255)
    - b: Blue (0-255)
    - a: Alpha (0-255, optional, defaults to 255 if omitted)
        
- Example:
    lua
    ```lua
    local custom_purple = {128, 0, 128, 255}  -- Purple, fully opaque
    rl.DrawRectangle(100, 100, 50, 50, custom_purple)
    ```
    

Using It

You can pass this directly to any raylib-luajit function that takes a color (e.g., DrawRectangle, ClearBackground, DrawText):

lua

```lua
rl.ClearBackground({50, 100, 150, 255})  -- Custom blue-gray
```

Predefined vs Custom

- Predefined: rl.RED, rl.GREEN, etc., are tables like {255, 0, 0, 255}.
    
- Custom: Define your own table anywhere in Lua and use it.
    

---

Cheatsheet: C to Lua API Comparison

Here’s a concise cheatsheet comparing the C APIs (from raylib, raygui, enet, raymath) to our Lua wrappers. I’ll cover key functions we’ve implemented, ensuring the Lua API aligns with C.

Raylib

| C Function                                                                     | Lua Function                                       | Notes                                  |
| ------------------------------------------------------------------------------ | -------------------------------------------------- | -------------------------------------- |
| void InitWindow(int width, int height, const char *title)                      | rl.InitWindow(width, height, title)                | Same args, no<br>return(void).         |
| bool WindowShouldClose(void)                                                   | rl.WindowShouldClose()                             | Returns<br>boolean.                    |
| void CloseWindow(void)                                                         | rl.CloseWindow()                                   | No args, no<br>return.                 |
| void BeginDrawing(void)                                                        | rl.BeginDrawing()                                  | No args, no<br>return.                 |
| void EndDrawing(void)                                                          | rl.EndDrawing()                                    | No args, no<br>return.                 |
| void ClearBackground(Color color)                                              | rl.ClearBackground(color)                          | color as<br>{r, g, b, a}<br>or rl.RED. |
| void DrawRectangle(int posX, int posY, int width, int height, Color color)     | rl.DrawRectangle(posX, posY, width, height, color) | Same args,<br>color as table.          |
| float GetFrameTime(void)                                                       | rl.GetFrameTime()                                  | Returns float<br>(delta time).         |
| void DrawText(const char *text, int posX, int posY, int fontSize, Color color) | rl.DrawText(text, posX, posY, fontSize, color)     | Same args,<br>color as table.          |
| bool IsKeyPressed(int key)                                                     | rl.IsKeyPressed(key)                               | Returns boolean, key as int.           |

Color Constants:

- C: RED, GREEN, RAYWHITE, etc.
- Lua: rl.RED, rl.GREEN, rl.RAYWHITE, etc. (tables like {255, 0, 0, 255}).
- Custom: {r, g, b, a} (e.g., {100, 150, 200, 255}).
    
---

Raygui

|C Function|Lua Function|Notes|
|---|---|---|
|int GuiButton(Rectangle bounds, const char *text)|rg.GuiButton(bounds, text)|bounds<br><br>as<br><br>{x, y, width, height}<br><br>,<br><br>returns<br><br>boolean<br><br>(clicked).|
|void GuiLabel(Rectangle bounds, const char *text)|rg.GuiLabel(bounds, text)|Same<br><br>args,<br><br>no<br><br>return.|
|int GuiCheckBox(Rectangle bounds, const char *text, bool *checked)|rg.GuiCheckBox(bounds, text, checked)|checked<br><br>is<br><br>boolean,<br><br>returns<br><br>new<br><br>state.|

Notes:

- Rectangle in C is {float x, y, width, height}; in Lua, it’s a table {x, y, width, height}.
    
- Raygui uses raylib’s drawing, so no extra color args here.
    

---

ENet

|C Function|Lua Function|Notes|
|---|---|---|
|int enet_initialize(void)|enet.initialize()|Returns<br><br>0<br><br>on<br><br>success,<br><br>else<br><br>error<br><br>code.|
|ENetHost *enet_host_create(const ENetAddress *address, size_t peerCount, size_t channelCount, enet_uint32 incomingBandwidth, enet_uint32 outgoingBandwidth)|enet.host_create(address, peerCount, channelCount, incomingBandwidth, outgoingBandwidth)|address<br><br>as<br><br>nil<br><br>or<br><br>{host, port, sin6_scope_id}<br><br>,<br><br>returns<br><br>userdata<br><br>or<br><br>nil.|
|ENetPeer *enet_host_connect(ENetHost *host, const ENetAddress *address, size_t channelCount, enet_uint32 data)|enet.host_connect(host, address, channelCount, data)|host<br><br>from<br><br>host_create<br><br>,<br><br>address<br><br>as<br><br>table.|
|ENetPacket *enet_packet_create(const void *data, size_t dataLength, enet_uint32 flags)|enet.packet_create(data, flags)|data<br><br>as<br><br>string,<br><br>flags<br><br>(1<br><br>=<br><br>reliable),<br><br>returns<br><br>userdata.|
|int enet_peer_send(ENetPeer *peer, enet_uint8 channelID, ENetPacket *packet)|enet.peer_send(peer, channelID, packet)|Returns<br><br>0<br><br>on<br><br>success,<br><br>else<br><br>error.|
|ENetEvent *enet_host_service(ENetHost *host, enet_uint32 timeout)|enet.host_service(host, timeout)|Returns<br><br>event<br><br>table<br><br>or<br><br>nil:<br><br>{type, peer, packet}<br><br>.|
|void enet_packet_destroy(ENetPacket *packet)|enet.packet_destroy(packet)|Destroys<br><br>packet<br><br>userdata.|
|const char *enet_packet_data(ENetPacket *packet)|enet.packet_data(packet)|Returns<br><br>string<br><br>data<br><br>from<br><br>packet.|
|void enet_host_destroy(ENetHost *host)|enet.host_destroy(host)|No<br><br>return.|
|void enet_deinitialize(void)|enet.deinitialize()|No<br><br>return.|

Event Types:

- C: ENET_EVENT_TYPE_CONNECT, ENET_EVENT_TYPE_RECEIVE, ENET_EVENT_TYPE_DISCONNECT.
    
- Lua: enet.EVENT_TYPE_CONNECT, enet.EVENT_TYPE_RECEIVE, enet.EVENT_TYPE_DISCONNECT.
    

---

Raymath

|C Function|Lua Function|Notes|
|---|---|---|
|Vector2 Vector2Add(Vector2 v1, Vector2 v2)|rm.Vector2Add(v1, v2)|v1<br><br>,<br><br>v2<br><br>as<br><br>{x, y}<br><br>,<br><br>returns<br><br>{x, y}<br><br>.|
|Vector2 Vector2Subtract(Vector2 v1, Vector2 v2)|rm.Vector2Subtract(v1, v2)|Same<br><br>format.|
|Vector2 Vector2Scale(Vector2 v, float scale)|rm.Vector2Scale(v, scale)|v<br><br>as<br><br>{x, y}<br><br>,<br><br>returns<br><br>{x, y}<br><br>.|
|float Vector2Length(Vector2 v)|rm.Vector2Length(v)|v<br><br>as<br><br>{x, y}<br><br>,<br><br>returns<br><br>float.|
|Vector2 Vector2Normalize(Vector2 v)|rm.Vector2Normalize(v)|v<br><br>as<br><br>{x, y}<br><br>,<br><br>returns<br><br>{x, y}<br><br>.|
|Vector3 Vector3Add(Vector3 v1, Vector3 v2)|rm.Vector3Add(v1, v2)|v1<br><br>,<br><br>v2<br><br>as<br><br>{x, y, z}<br><br>,<br><br>returns<br><br>{x, y, z}<br><br>.|
|Vector3 Vector3Subtract(Vector3 v1, Vector3 v2)|rm.Vector3Subtract(v1, v2)|Same<br><br>format.|
|Vector3 Vector3Scale(Vector3 v, float scale)|rm.Vector3Scale(v, scale)|v<br><br>as<br><br>{x, y, z}<br><br>,<br><br>returns<br><br>{x, y, z}<br><br>.|
|Matrix MatrixIdentity(void)|rm.MatrixIdentity()|Returns<br><br>16-element<br><br>table<br><br>(4x4<br><br>matrix).|
|Matrix MatrixMultiply(Matrix m1, Matrix m2)|rm.MatrixMultiply(m1, m2)|m1<br><br>,<br><br>m2<br><br>as<br><br>16-element<br><br>tables,<br><br>returns<br><br>same.|

Formats:

- Vector2: {x, y} (e.g., {10, 20}).
    
- Vector3: {x, y, z} (e.g., {10, 20, 30}).
    
- Vector4: {x, y, z, w} (e.g., {10, 20, 30, 40}).
    
- Matrix: {m0, m4, m8, m12, m1, m5, m9, m13, m2, m6, m10, m14, m3, m7, m11, m15} (row-major 4x4).

API Match Confirmation

The cheatsheet shows our Lua wrappers closely mirror the C APIs:

- Args: Converted structs (Color, Rectangle, etc.) to Lua tables.
    
- Returns: Adjusted to Lua conventions (e.g., bool instead of int where it makes sense).
    
- Naming: Kept function names identical, prefixed with module (rl., rg., etc.).