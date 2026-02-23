// entities.hpp

#pragma once

#include "raylib.h"

struct sheep {
	sheep();

	void update(float dt);
	void render() const;
	void sense();
	void checkSheep();
	void checkWolf();
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
	Vector2 acceleration;
};

struct wolf {
	wolf();

	void update(float dt);
	void render() const;
	void sense();
	void checkSheep();
	void attack();


	float hunger;
	float speed;
	float detection_radius;
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 denposition;
};

