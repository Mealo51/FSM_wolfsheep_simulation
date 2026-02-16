// application.cpp

#include "application.hpp"
#include "common.hpp"
#include "collision.hpp"

App::App(int width, int height)
{
   // note: screen bounds
   bounds = Vector2{ (float)width, (float)height };

   // note: initialize aligned rectangle
   aligned.position = bounds * 0.3f;
   aligned.extent = Vector2{ 50.0f, 20.0f };

   // note: initialize rotated rectangle
   rotated.position = bounds * 0.7f;
   rotated.extent = Vector2{ 10.0f, 90.0f };

   // note: initialize all circles
   constexpr float RADIUS = 8.0f;
   constexpr float VELOCITY = 100.0f;
   for (auto& circle : circles) {
      circle.position = Random::position_in_bounds({ RADIUS, RADIUS }, { width - RADIUS, height - RADIUS });
      circle.radius   = RADIUS;
      circle.velocity = Random::direction() * VELOCITY;
   }

   // note: input mapping
   m_input.add_key_binding("jump", KEY_SPACE);
   m_input.add_axis_binding("test", KEY_A, KEY_D);
   m_input.add_direction_binding("move", KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN);
   m_input.save("data/mapping.txt");
}

static void constrain_within_bounds(Circle& circle, Vector2 bounds)
{
   // note: stay within screen-x please
   if (circle.position.x < circle.radius) {
      circle.position.x = circle.radius;
      circle.velocity.x = -circle.velocity.x;
   }
   else if (circle.position.x > bounds.x - circle.radius) {
      circle.position.x = bounds.x - circle.radius;
      circle.velocity.x = -circle.velocity.x;
   }

   // note: stay within screen-y please
   if (circle.position.y < circle.radius) {
      circle.position.y = circle.radius;
      circle.velocity.y = -circle.velocity.y;
   }
   else if (circle.position.y > bounds.y - circle.radius) {
      circle.position.y = bounds.y - circle.radius;
      circle.velocity.y = -circle.velocity.y;
   }
}

static void check_collisions(App& app)
{
   constexpr Color COLLIDING_COLOR = PINK;

   for (auto& outer : app.circles) {
      // note: circle vs aligned rectangle
      if (Collision::overlap(outer, app.aligned)) {
         outer.color = COLLIDING_COLOR;
         app.aligned.color = COLLIDING_COLOR;
      }
      
      // note: circle vs rotated rectangle
      if (Collision::overlap(outer, app.rotated)) {
         outer.color = COLLIDING_COLOR;
         app.rotated.color = COLLIDING_COLOR;
      }
      
      // note: circle vs all other circles
      for (auto& inner : app.circles) {
         // note: skip checking against self
         if (&outer == &inner) {
            continue;
         }

         if (Collision::overlap(outer, inner)) {
            outer.color = COLLIDING_COLOR;
            inner.color = COLLIDING_COLOR;
         }
      }
   }
}

void App::update(float dt)
{
   m_input.update();

   constexpr float SPEED = 100.0f;
   Vector2 direction = m_input.value("move");
   aligned.position = aligned.position + direction * SPEED * dt;
   aligned.update(dt);

   rotated.update(dt);

   for (auto& circle : circles) {
      circle.update(dt);

      constrain_within_bounds(circle, bounds);
   }

   check_collisions(*this);
}

void App::render()
{
   aligned.render();
   rotated.render();
   for (auto& circle : circles) {
      circle.render();
   }
}
