//grass.hpp
#pragma once

#include "raylib.h"

enum class GrassState {
	growing,
	growing_fast,
	grown,
	spreading,
	wilting,
};

struct grass {
	grass(Vector2 pos);

	void update(float dt);
	void render() const;
	void checkState();
	void handleState();

	float growth_rate;
	float grow_progress;
	float grown_countdown;
	float death_countdown;
	int spread_attempts;
	bool near_manure;
	GrassState state;
	Vector2 position;
};