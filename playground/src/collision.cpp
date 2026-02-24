// collision.cpp

#include "collision.hpp"
#include "common.hpp"
#include "Constant.hpp"

namespace Collision
{
	inline bool checkSheepWolf(const sheep& s, const wolf& w)
	{
		return CheckCollisionCircles(s.position, sheep_radius, w.position, wolf_radius);
	}

	inline bool checkSheepSheep(const sheep& s1, const sheep& s2)
	{
		return CheckCollisionCircles(s1.position, sheep_radius, s2.position, sheep_radius);
	}
} // !Collision
