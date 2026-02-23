//grass.cpp

#include "raylib.h"
#include "common.hpp"
#include "Constant.hpp"
#include "grass.hpp"

grass::grass(Vector2 pos)
{
	growth_rate = 1.f * tick_rate; 
	grow_progress = 0.f;
	state = Random::range(0, 1) == 0 ? GrassState::growing : GrassState::grown;
	position = pos;
}

void grass::update(float dt)
{
	checkState();
}