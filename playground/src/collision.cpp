// collision.cpp

#include "collision.hpp"
#include "common.hpp"
#include "Constant.hpp"

namespace Collision
{
	bool searchSheepWolf(const sheep& s, const wolf& w)
	{
		return CheckCollisionCircles(s.position, s.detection_radius, w.position, wolf_radius);
	}

	bool checkSheepWolf(const sheep& s, const wolf& w)
	{
		return CheckCollisionCircles(s.position, sheep_radius, w.position, wolf_radius);
	}

	bool searchWolfSheep(const wolf& w, const sheep& s)
	{
		return CheckCollisionCircles(w.position, w.detection_radius, s.position, sheep_radius);
	}
	bool checkWolfSheep(const wolf& w, const sheep& s)
	{
		return CheckCollisionCircles(w.position, wolf_radius, s.position, sheep_radius);
	}

	bool checkSheepSheep(const sheep& s1, const sheep& s2)
	{
		return CheckCollisionCircles(s1.position, s1.detection_radius, s2.position, s2.detection_radius);
	}

	bool checkSheepGrass(const sheep& s, const grass& g)
	{
		return CheckCollisionCircles(s.position, sheep_radius, g.position, tile_len / 2);
	}

	bool checkGrassManure(const grass& g, const manure& m)
	{
		return CheckCollisionCircles(g.position, tile_len * 2, m.position, 5.f);
	}

	bool checkSheepManure(const sheep& s, const manure& m)
	{
		return CheckCollisionCircles(s.position, sheep_radius, m.position, 5.f);
	}

	bool checkSheepWindow(const sheep& s, const Vector2& bounds)
	{
		return (s.position.x - sheep_radius < 0 || s.position.x + sheep_radius > bounds.x ||
			s.position.y - sheep_radius < 0 || s.position.y + sheep_radius > bounds.y);
	}

	bool checkWolfWindow(const wolf& w, const Vector2& bounds)
	{
		return (w.position.x - wolf_radius < 0 || w.position.x + wolf_radius > bounds.x ||
			w.position.y - wolf_radius < 0 || w.position.y + wolf_radius > bounds.y);
	}
} // !Collision
