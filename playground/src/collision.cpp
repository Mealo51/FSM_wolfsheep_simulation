// collision.cpp

#include "collision.hpp"
#include "common.hpp"

namespace Collision
{
   bool overlap(const Circle& lhs, const Circle& rhs)
   {
      return false;
   }

   bool overlap(const Circle& lhs, const AlignedRectangle& rhs)
   {
      return false;
   }

   bool overlap(const Circle& lhs, RotatedRectangle& rhs)
   {
      return false;
   }
} // !Collision
