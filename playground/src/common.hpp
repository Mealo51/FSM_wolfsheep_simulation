// common.hpp

#pragma once

#include "raymath.h"


// note: Vector2 sanity operators
inline Vector2 operator- (const Vector2& rhs)                     { return Vector2{ -rhs.x, -rhs.y }; }
inline Vector2 operator+ (const Vector2& lhs, const Vector2& rhs) { return Vector2{ lhs.x + rhs.x, lhs.y + rhs.y }; }
inline Vector2 operator- (const Vector2& lhs, const Vector2& rhs) { return Vector2{ lhs.x - rhs.x, lhs.y - rhs.y }; }
inline Vector2 operator* (const float lhs, const Vector2& rhs)    { return Vector2{ lhs * rhs.x, lhs * rhs.y }; }
inline Vector2 operator/ (const float lhs, const Vector2& rhs)    { return Vector2{ lhs / rhs.x, lhs / rhs.y }; }
inline Vector2 operator* (const Vector2& lhs, const float rhs)    { return Vector2{ lhs.x * rhs, lhs.y * rhs }; }
inline Vector2 operator/ (const Vector2& lhs, const float rhs)    { return Vector2{ lhs.x / rhs, lhs.y / rhs }; }
inline float   dot(const Vector2 &lhs, const Vector2& rhs)        { return Vector2DotProduct(lhs, rhs); }
inline float   dot_perp(const Vector2 &lhs, const Vector2 &rhs)   { return lhs.x * rhs.y - lhs.y * rhs.x; }
inline float   length(const Vector2 &rhs)                         { return Vector2Length(rhs); }
inline float   length_squared(const Vector2 &rhs)                 { return Vector2LengthSqr(rhs); }
inline Vector2 normalize(const Vector2& rhs)                      { return Vector2Normalize(rhs); }
inline Vector2 rotate(const Vector2& lhs, const float radians)    { return Vector2Transform(lhs, MatrixRotateZ(radians)); }
inline Vector2 perpendicular_cw(const Vector2 &rhs)               { return Vector2{ rhs.y, -rhs.x }; }
inline Vector2 perpendicular_ccw(const Vector2 &rhs)              { return Vector2{ -rhs.y, rhs.x }; }

namespace Math {
   constexpr float kPI  = 3.1415927f;
   constexpr float kTAU = (2.0f * kPI);
} // !Math

namespace Random {
   static unsigned int seed = 8499563u;

   inline int next()
   {
      seed = seed * 1103515245u + 12345u; 
      return (int)(seed & 0x7fff); 
   }

   inline int range(int lo, int hi) 
   {
      return lo + next() % ((hi - lo) + 1); 
   }

   inline float range01() { 
      return next() / 32767.0f; 
   }

   inline float rangef(float lo, float hi) 
   {
      return lo + (hi - lo) * range01(); 
   }

   inline Vector2 direction() 
   {
      const float r = range01() * Math::kTAU; 
      return Vector2{ cosf(r), sinf(r) }; 
   }

   inline Vector2 rdirection()
   {
	   const float r = static_cast<float>(GetRandomValue(0, 360)) * (Math::kPI / 180.f);
	   return Vector2{ cosf(r), sinf(r) };
   }

   inline Vector2 position_in_bounds(const Vector2& min, const Vector2& max) 
   { 
      return Vector2{ rangef(min.x, max.x), rangef(min.y, max.y) }; 
   }
} // !Random
