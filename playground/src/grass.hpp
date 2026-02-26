//grass.hpp
#pragma once

#include "raylib.h"
#include "Constant.hpp"
#include <array>

enum class GrassState {
	growing,
	growing_fast,
	grown,
	spreading,
	wilting,
	dirt
};

struct grass {
	grass(Vector2 pos);

	void update(float dt);
	void render() const;
	void checkState();
	void handleState();

	float growth_rate;
	float grow_progress;
	float spread_countdown; 
	//to leave a state for spreading otherwise it just jumps straight into wilting from grown 
	//(grown to spread to wilt but spread goes to wilt instantly)
	float grown_countdown;
	float death_countdown;

	//spread
	int spread_attempts;
	std::array<int, 2> spread_indices;

	bool near_manure;
	GrassState state;
	Vector2 position;
	Vector2 bounds{ tile_len, tile_len };
};