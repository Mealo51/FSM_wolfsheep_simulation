// collision.cpp

#include "collision.hpp"
#include "common.hpp"
#include "Constant.hpp"

namespace Collision
{
	//wolf
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

	bool SearchWolfGrass(const wolf& w, const grass& g)
	{
		return CheckCollisionCircleRec(w.position, w.detection_radius, g.rect);
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
		// 1. Create a smaller "eating zone" in the center of the tile
	// If tile_len is 32, a margin of 10 means the sheep must be in the middle 12x12 pixels
		float margin = tile_len * 0.3f;

		Rectangle centerZone = {
			g.position.x + margin,
			g.position.y + margin,
			tile_len - (margin * 2),
			tile_len - (margin * 2)
		};

		// 2. Check if the sheep's center point is inside this small zone
		// Using CheckCollisionPointRec is stricter than CircleRec
		return CheckCollisionPointRec(s.position, centerZone);
	}

	bool checkGrassManure(const grass& g, const manure& m)
	{
		Rectangle grassRec = {
			g.position.x ,
			g.position.y ,
			tile_len,
			tile_len
		};
		return CheckCollisionCircleRec(m.position, manure_radius, grassRec);
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
