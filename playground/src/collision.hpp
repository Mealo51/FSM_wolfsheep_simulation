// collision.hpp

#pragma once

#include "entities.hpp"
#include "grass.hpp"
#include "Constant.hpp"

namespace Collision
{
	bool checkSheepWolf(const sheep& s, const wolf& w);
	bool checkWolfSheep(const wolf& w, const sheep& s);
	bool checkSheepSheep(const sheep& s1, const sheep& s2);
	bool checkSheepGrass(const sheep& s, const grass& g);
	bool checkGrassManure(const grass& g, const manure& m);
	bool checkSheepWindow(const sheep& s, const Vector2& bounds);
	bool checkWolfWindow(const wolf& w, const Vector2& bounds);
} // !Collision