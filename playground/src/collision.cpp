// collision.cpp

#include "collision.hpp"
#include "common.hpp"
#include "Constant.hpp"

namespace Collision
{
	//sheep wolf
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

	//sheep sheep
	bool searchSheepSheep(const sheep& s1, const sheep& s2)
	{
		return CheckCollisionCircles(s1.position, s1.detection_radius, s2.position, sheep_radius);
	}

	bool checkSheepSheep(const sheep& s1, const sheep& s2)
	{
		return CheckCollisionCircles(s1.position, sheep_radius + 0.25f, s2.position, sheep_radius + 0.25f);
	}

	//sheep grass, grass and manure
	bool checkSheepGrass(const sheep& s, const grass& g)
	{
		return CheckCollisionCircleRec(s.position, 2.f, g.boundsRec); 
		//make sheep only eat when it is near center so it doesn't eat from the edge of the tile
	}

	bool checkGrassManure(const grass& g, const manure& m)
	{
		return CheckCollisionCircleRec(m.position, manure_radius, g.boundsRec);
	}

	bool checkSheepManure(const sheep& s, const manure& m)
	{
		return CheckCollisionCircles(s.position, sheep_radius, m.position, manure_radius);
	}


	//window bounds
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
