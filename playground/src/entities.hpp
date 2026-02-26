// entities.hpp

#pragma once

#include "raylib.h"

enum class sheepState {
	roaming,
	eating,
	fleeing,
	reproducing,
	defecating,
};

struct sheep {
	sheep();

	void update(float dt, Vector2 wolfpos);
	void render() const;
	void checkState();
	void handleState();

	sheep reproduce();
	void eatGrass();
	void defecate();

	Vector2 flee(Vector2 wolfPos);
	Vector2 roam();

	float HP;
	float fullness;
	float speed;
	float reproduce_cd;
	float detection_radius;
	bool nearWolf;
	bool nearSheep;
	bool nearGrass;
	bool nearManure;
	sheepState state;
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
};

enum class wolfState {
	roaming,
	attacking,
	sleeping,
};

struct wolf {
	wolf();

	void update(float dt);
	void render() const;
	void checkState();
	void handleState();

	bool checkSheep();
	bool attack();


	float hunger;
	float speed;
	float detection_radius;
	wolfState state;
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 denposition;
};

