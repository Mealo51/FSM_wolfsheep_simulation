//grass.cpp

#include "raylib.h"
#include "common.hpp"
#include "Constant.hpp"
#include "grass.hpp"

grass::grass(Vector2 pos)
{
	growth_rate = 1.f * tick_rate; //default growth rate, can be modified by manure
	grow_progress = 0.f;
	death_countdown = 0.f;
	grown_countdown = 0.f;
	spread_attempts = 0;
	near_manure = false;
	state = Random::range(0, 1) == 0 ? GrassState::growing : GrassState::grown;
	position = pos;
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
		DrawRectangleV(position, bounds, YELLOW);
		break;
	case GrassState::grown:
		DrawRectangleV(position, bounds, GREEN);
		break;
	case GrassState::spreading:
		DrawRectangleV(position, bounds, BLUE);
		break;
	case GrassState::wilting:
		DrawRectangleV(position, bounds, BROWN);
		break;
	}
}

void grass::handleState() {
	switch (state) {
	case GrassState::growing:
		grow_progress += growth_rate;
		break;
	case GrassState::growing_fast:
		grow_progress += growth_rate * 2.f;
		break;
	case GrassState::spreading:
		// Logic to check empty neighboring tiles
		//spread()
		state = GrassState::grown;
		break;
	case GrassState::wilting:
		death_countdown += tick_rate; // after 10sec world should delete this grass tile
		break;
	case GrassState::grown:
		grown_countdown += tick_rate; // after 20sec grass should start wilting
		break;
	}
}

void grass::checkState() {
	switch (state) {
	case GrassState::growing:
		if (grow_progress >= 1200.0f) state = GrassState::grown;
		if (near_manure) state = GrassState::growing_fast;
		break;

	case GrassState::growing_fast:
		if (grow_progress >= 600.0f) state = GrassState::grown;
		if (!near_manure) state = GrassState::growing;
		break;

	case GrassState::grown:
		// A grown tile might randomly decide to spread
		for (int i = 0; i < GetRandomValue(0, 4); i++) { //randomly choose 1-5 times to check spreading
			if (GetRandomValue(0, 1000) > 750) spread_attempts++;
		}
		if (spread_attempts > 0) state = GrassState::spreading;
		break;

	case GrassState::spreading:
		for (int i = 0; i < spread_attempts; i++) {
			GetRandomValue(1, 8); // this will be used to determine which neighboring tile to spread to
			//spread to the chosen tile if it's empty
		}
		break;
	}
}