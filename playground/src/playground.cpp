// playground.cpp

#include "raylib.h"
#include "application.hpp"
#include "Constant.hpp"

int main()
{
   // note: config

   // note: initialize
   InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Playground");
   SetTargetFPS(fps_lim);
   //InitAudioDevice();

   App app(WINDOW_WIDTH, WINDOW_HEIGHT);

   while (!WindowShouldClose()) {
      ClearBackground(DARKBROWN);
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
