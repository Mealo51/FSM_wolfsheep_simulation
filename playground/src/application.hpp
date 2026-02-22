// application.hpp

#pragma once

#include "input.hpp"
#include "entities.hpp"

constexpr int CIRCLE_COUNT = 128;

struct App {
   App(int width, int height);

   void update(float dt);
   void render();

   Vector2 bounds{ 0,0 };

   sheep s1;

   InputContext m_input;
};

