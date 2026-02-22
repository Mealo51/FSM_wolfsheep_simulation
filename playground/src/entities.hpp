// entities.hpp

#pragma once

#include "raylib.h"

struct sheep {
	sheep() = default;

	void update(float dt);
	void render() const;
	void checkSheep(sheep s);
	void reproduce();
	void searchGrass();
	void eatGrass();
	void defecate();

	float HP;
	float fullness;
	float speed;
	float reproduce_cd;
	float detection_radius;
	bool wolfNearby;
	Vector2 position;
	Vector2 velocity;
};

struct wolf {
	wolf() = default;

	void update(float dt);
	void render() const;
	void checkSheep();
	void attack();


	float hunger;
	float speed;
	float detection_radius;
	Vector2 position;
	Vector2 velocity;
};

