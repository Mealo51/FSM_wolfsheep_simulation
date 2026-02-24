// entities.hpp

#pragma once

#include "raylib.h"

enum class sheepState {
	idle,
	roaming,
	eating,
	fleeing,
	reproducing,
};

struct sheep {
	sheep();

	void update(float dt);
	void render() const;
	void checkState();
	void handleState();
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
	sheepState state;
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
};

enum class wolfState {
	idle,
	roaming,
	hunting,
	attacking,
	sleeping,
};

struct wolf {
	wolf();

	void update(float dt);
	void render() const;
	void checkState();
	void checkSheep();
	void attack();


	float hunger;
	float speed;
	float detection_radius;
	wolfState state;
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 denposition;
};

