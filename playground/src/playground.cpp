// playground.cpp

#include "raylib.h"
#include "application.hpp"

int main()
{
   // note: config
   constexpr int WINDOW_WIDTH = 1024;
   constexpr int WINDOW_HEIGHT = 1024;

   // note: initialize
   InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Playground");
   //InitAudioDevice();

   App app(WINDOW_WIDTH, WINDOW_HEIGHT);

   while (!WindowShouldClose()) {
      ClearBackground(RAYWHITE);
      BeginDrawing();

      float dt = GetFrameTime();
      app.update(dt);
      app.render();

      DrawFPS(2, 2);
      EndDrawing();
   }

   // note: shutdown
   //CloseAudioDevice();
   CloseWindow();

   return 0;
}
