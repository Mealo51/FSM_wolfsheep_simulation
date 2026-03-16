// collision.hpp

#pragma once

#include "entities.hpp"
#include "grass.hpp"
#include "Constant.hpp"

namespace Collision
{
	//wolf
	bool searchSheepWolf(const sheep& s, const wolf& w);
	bool checkSheepWolf(const sheep& s, const wolf& w);
	bool searchWolfSheep(const wolf& w, const sheep& s);
	bool checkWolfSheep(const wolf& w, const sheep& s);
	bool checkWolfGrass(const wolf& w, const grass& g);
	bool searchWolfGrass(const wolf& w, const grass& g);
	//sheep sheep
	bool searchSheepSheep(const sheep& s1, const sheep& s2);
	bool checkSheepSheep(const sheep& s1, const sheep& s2);
	//sheep grass, grass and manure
	bool checkSheepGrass(const sheep& s, const grass& g);
	bool searchSheepGrass(const sheep& s, const grass& g);
	bool checkGrassManure(const grass& g, const manure& m);
	bool checkSheepManure(const sheep& s, const manure& m);

	//window
	bool checkSheepWindow(const sheep& s, const Vector2& bounds);
	bool checkWolfWindow(const wolf& w, const Vector2& bounds);
} // !Collision