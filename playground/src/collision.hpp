// collision.hpp

#pragma once

#include "entities.hpp"
#include "Constant.hpp"

namespace Collision
{
	inline bool checkSheepWolf(const sheep& s, const wolf& w);
	inline bool checkSheepSheep(const sheep& s1, const sheep& s2);
} // !Collision