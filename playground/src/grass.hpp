//grass.hpp
#pragma once

#include "raylib.h"
#include "Constant.hpp"
#include <array>

struct App;

enum class GrassState {
	growing,
	growing_fast,
	grown,
	wilting,
	dirt
};

struct grass {
	grass(Vector2 pos);

	void update(float dt, App& app);
	void render() const;

	void sense(App& app);
	void decide();
	void act(float dt, App& app);
	float sensecd;
	float decidecd;

	float growth_rate;
	float grow_progress;
	float grown_countdown;
	float death_countdown;

	//spread
	int spread_attempts;
	std::array<int, 2> spread_indices;

	bool near_manure;
	GrassState state;
	Vector2 position;
	Vector2 bounds{ tile_len, tile_len };
	Rectangle boundsRec{ position.x, position.y, bounds.x, bounds.y };
};