// entities.hpp

#pragma once

#include "raylib.h"

struct Circle {
   Circle() = default;

   void update(float dt);
   void render() const;

   Vector2 position{ 0,0 };
   float   radius{ 0 };
   Vector2 velocity{ 0,0 };
   Color   color{ 0 };
};

struct AlignedRectangle {
   AlignedRectangle() = default;

   void update(float dt);
   void render() const;

   Vector2 position{ 0,0 };
   Vector2 extent{ 0,0 };
   Color   color{ 0 };
};

struct RotatedRectangle {
   RotatedRectangle() = default;

   void update(float dt);
   void render() const;

   Vector2 position{ 0,0 };
   Vector2 extent{ 0,0 };
   float   rotation{ 0 };
   Color   color{ 0 };
};
