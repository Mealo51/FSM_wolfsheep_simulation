//grass.cpp

#include "raylib.h"
#include "common.hpp"
#include "Constant.hpp"
#include "grass.hpp"
#include "application.hpp"
#include "collision.hpp"

grass::grass(Vector2 pos)
{
	growth_rate = 1.f ; //default growth rate, can be modified by manure
	death_countdown = 0.f;
	grown_countdown = 0.f;
	spread_attempts = 0;
	near_manure = false;
	state = GetRandomValue(0, 9) <= 5 ? GrassState::growing :
		GetRandomValue(0, 9) <= 3 ? GrassState::grown : GrassState::wilting;
	if (state == GrassState::growing)
	{
		grow_progress = static_cast<float>(GetRandomValue(0, 20));
	}
	else
	{
		grow_progress = 0.f;
	}
	position = pos;
	spread_indices = { -1, -1 };

	sensecd = 0.f;
	decidecd = 0.f;
}

void grass::update(float dt, App& app)
{
	sensecd +=  dt;
	decidecd +=  dt;
	if (sensecd >= 0.25f) {
		sensecd = 0.f;
		sense(app);
	}
	if (decidecd >= 0.5f) {
		decidecd = 0.f;
		decide();
	}
	act(dt, app);
}

void grass::render() const
{
	switch (state) {
	case GrassState::growing:
	case GrassState::growing_fast:
		DrawRectangleV(position, bounds, GREEN);
		DrawText(TextFormat("Growth:\n %.1f%%", (grow_progress/ total_growtime) * 100.f),
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


void grass::sense(App& app)
{
	near_manure = false;
	for (const auto& m : app.m_manure) {
		if (Collision::checkGrassManure(*this, m)) {
			near_manure = true;
			break;
		}
	}
	for(auto& s : app.m_sheep) {
		if (Collision::checkSheepGrass(s, *this) && s.eating && state == GrassState::grown) {
			state = GrassState::dirt;
			break;
		}
	}
}

void grass::decide()
{
	switch (state) {
	case GrassState::growing:
		if (grow_progress >= total_growtime) state = GrassState::grown;
		if (near_manure) state = GrassState::growing_fast;
		break;
	case GrassState::growing_fast:
		if (grow_progress >= total_growtime) state = GrassState::grown;
		if (!near_manure) state = GrassState::growing;
		break;
	case GrassState::grown:
		if (grown_countdown >= 20.f) state = GrassState::wilting;
		break;
	case GrassState::wilting:
		if (death_countdown >= 10.f) state = GrassState::dirt;
		break;
	case GrassState::dirt:
		break;
	}
}

void grass::act(float dt, App& app)
{
	switch (state) {
	case GrassState::growing:
		grow_progress += growth_rate * dt; // Grows 1.0 unit per second
		break;
	case GrassState::growing_fast:
		grow_progress += growth_rate * dt * 2.0f; // Manure doubles growth speed
		break;
	case GrassState::grown:
		grown_countdown += dt;
		if (grown_countdown >= 10.f) {
			for (int i = 0; i < GetRandomValue(0, 3); i++)
			{ //randomly choose 0-2 times to check spreading
				if (GetRandomValue(0, 1000) > 800 && spread_attempts < 2) spread_attempts++;
			}
		}
		break;

	case GrassState::wilting:
		death_countdown += dt;
		break;
	}
}