// collision.cpp

#include "collision.hpp"
#include "common.hpp"
#include "Constant.hpp"

namespace Collision
{
	bool checkSheepWolf(const sheep& s, const wolf& w)
	{
		return CheckCollisionCircles(s.position, sheep_radius, w.position, wolf_radius);
	}

	bool checkSheepSheep(const sheep& s1, const sheep& s2)
	{
		return CheckCollisionCircles(s1.position, sheep_radius, s2.position, sheep_radius);
	}

	bool checkSheepWindow(const sheep& s, const Vector2& bounds)
	{
		return (s.position.x - sheep_radius < 0 || s.position.x + sheep_radius > bounds.x ||
			s.position.y - sheep_radius < 0 || s.position.y + sheep_radius > bounds.y);
	}
} // !Collision
