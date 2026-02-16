// collision.hpp

#pragma once

#include "entities.hpp"

namespace Collision
{
   bool overlap(const Circle& lhs, const Circle& rhs);
   bool overlap(const Circle& lhs, const AlignedRectangle& rhs);
   bool overlap(const Circle& lhs, RotatedRectangle& rhs);
} // !Collision
