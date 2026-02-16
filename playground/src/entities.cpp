// entities.cpp

#include "entities.hpp"
#include "common.hpp"

constexpr Color NON_COLLIDING_COLOR = LIME;

void Circle::update(float dt)
{
   color = NON_COLLIDING_COLOR;
   position = position + velocity * dt;
}

void Circle::render() const
{
   DrawCircleLinesV(position, radius, color);
}

void AlignedRectangle::update(float dt)
{
   color = NON_COLLIDING_COLOR;
}

void AlignedRectangle::render() const
{
   const Rectangle rect{
      position.x - extent.x,
      position.y - extent.y,
      extent.x * 2.0f,
      extent.y * 2.0f
   };
   DrawRectangleLinesEx(rect, 1.0f, color);
}

void RotatedRectangle::update(float dt)
{
   constexpr float ROTATION_SPEED = Math::kTAU / 10.0f;

   color = NON_COLLIDING_COLOR;
   rotation += ROTATION_SPEED * dt;
   if (rotation > Math::kTAU) {
      rotation -= Math::kTAU;
   }
}

void RotatedRectangle::render() const
{
   Matrix m = MatrixRotateZ(rotation);
   Vector2 corners[4] = {
      { -extent.x, -extent.y },
      {  extent.x, -extent.y },
      {  extent.x,  extent.y },
      { -extent.x,  extent.y }
   };

   for (auto& corner : corners) {
      corner = position + Vector2Transform(corner, m);
   }

   for (int index = 0; index < 4; index++) {
      DrawLineV(corners[index], corners[(index + 1) & 0x3], color);
   }
}
