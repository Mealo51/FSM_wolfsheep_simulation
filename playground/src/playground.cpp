// playground.cpp

#include "raylib.h"
#include "application.hpp"

int main()
{
   // note: config
   constexpr int WINDOW_WIDTH = 1024;
   constexpr int WINDOW_HEIGHT = 1024;
   constexpr int fps_lim = 60;
   // note: initialize
   InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Playground");
   SetTargetFPS(fps_lim);
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
