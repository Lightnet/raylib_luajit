local rl = require("rl")

rl.InitWindow(800, 450, "raylib [core] example - basic window")

--rl.SetTargetFPS(60)

while not rl.WindowShouldClose() do

  rl.BeginDrawing()
  rl.ClearBackground(rl.RAYWHITE)
  rl.DrawText("Congrats! You created your first window!", 190, 200, 20, rl.BLACK)
  rl.EndDrawing()

end

rl.CloseWindow()