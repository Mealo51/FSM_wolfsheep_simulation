// application.hpp

#pragma once

#include "input.hpp"
#include "Constant.hpp"
#include "entities.hpp"
#include "grass.hpp"

struct App {
   App(int width, int height);

   void update(float dt);
   void render();

   Vector2 bounds{ 0,0 };
   InputContext m_input;

   grass grass_tiles = grass({200,200});
};

