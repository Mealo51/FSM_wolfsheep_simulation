//grass.cpp

#include "raylib.h"
#include "common.hpp"
#include "Constant.hpp"
#include "grass.hpp"

grass::grass(Vector2 pos)
{
	growth_rate = 1.f * tick_rate; //default growth rate, can be modified by manure
	death_countdown = 0.f;
	grown_countdown = 0.f;
	spread_attempts = 0;
	near_manure = false;
	state = GetRandomValue(0, 9) <= 5 ? GrassState::growing :
		GetRandomValue(0, 9) <= 3 ? GrassState::grown : GrassState::wilting;
	if (state == GrassState::growing)
	{
		grow_progress = static_cast<float>(GetRandomValue(0, 300));
	}
	position = pos;
	spread_indices = { -1, -1 };
}

void grass::update(float dt)
{
	checkState();
	handleState();
}

void grass::render() const
{
	switch (state) {
	case GrassState::growing:
	case GrassState::growing_fast:
		DrawRectangleV(position, bounds, GREEN);
		DrawText(TextFormat("Growth: %.1f%%", grow_progress / 600.f * 100.f),
			static_cast<int>(position.x) + 5, static_cast<int>(position.y) + 5, 10, BLACK);
		break;
	case GrassState::grown:
		DrawRectangleV(position, bounds, LIME);
		DrawText("Grown", static_cast<int>(position.x) + 5, static_cast<int>(position.y) + 5, 10, BLACK);
		break;
	case GrassState::wilting:
		DrawRectangleV(position, bounds, YELLOW);
		DrawText("Wilting", static_cast<int>(position.x) + 5,
			static_cast<int>(position.y) + 5, 10, BLACK);
		break;
	}//dirt state is not rendered, it's just empty space
}


void grass::handleState()
{
	switch (state) {
	case GrassState::growing:
		grow_progress += growth_rate;
		break;
	case GrassState::growing_fast:
		grow_progress += growth_rate * 2.f;
		break;
	case GrassState::wilting:
		death_countdown += tick_rate; // after 10sec world should delete this grass tile
		if (death_countdown >= 600.f) state = GrassState::dirt;
		break;
	case GrassState::grown:
		grown_countdown += tick_rate; // after 10sec grass should start wilting
		if (grown_countdown >= 600.f)
		{
			state = GrassState::wilting;
		}
		break;
	case GrassState::dirt:
		// do nothing, this tile is just dirt
		break;
	}
}

void grass::checkState() {
	switch (state) {
	case GrassState::growing:
		if (grow_progress >= 600.0f) state = GrassState::grown;
		if (near_manure) state = GrassState::growing_fast;
		break;
	case GrassState::growing_fast:
		if (grow_progress >= 300.0f) state = GrassState::grown;
		if (!near_manure) state = GrassState::growing;
		break;
	case GrassState::grown:
		if (grown_countdown >= 600.0f)
		{
			// A grown tile might randomly decide to spread
			for (int i = 0; i < GetRandomValue(0, 3); i++)
			{ //randomly choose 0-2 times to check spreading
				if (GetRandomValue(0, 1000) > 800 && spread_attempts < 2) spread_attempts++;
				DrawText(TextFormat("Spread attempts: %d", spread_attempts), 10, 10, 20, BLACK);
			}
		}
		break;
	}
}