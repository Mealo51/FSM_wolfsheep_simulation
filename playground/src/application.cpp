// application.cpp

#include "application.hpp"
#include "common.hpp"
#include "collision.hpp"

App::App(int width, int height)
{
   // note: screen bounds
   bounds = Vector2{ (float)width, (float)height };

   // note: input mapping
   m_input.add_key_binding("jump", KEY_SPACE);
   m_input.add_axis_binding("test", KEY_A, KEY_D);
   m_input.add_direction_binding("move", KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN);
   m_input.save("data/mapping.txt");
}


static void check_collisions(App& app)
{
   
}

void App::update(float dt)
{
   m_input.update();
   s1.update(dt);
   constexpr float SPEED = 100.0f;
   Vector2 direction = m_input.value("move");
   check_collisions(*this);
}

void App::render()
{
	s1.render();
}
