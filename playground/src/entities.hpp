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

	void update(float dt, Vector2 wolfpos, Vector2 sheeppos);
	void render() const;
	void checkState();
	void handleState(Vector2 wolfpos, Vector2 sheeppos);

	sheep reproduce();
	void eatGrass();
	void defecate();

	Vector2 flee(Vector2 wolfPos);
	Vector2 roam();
	Vector2 drag();
	Vector2 cohesion(Vector2 sheeppos);
	float fleeweight;
	float roamweight;
	float dragweight;
	float cohesionweight;
	float speed;
	float max_speed;

	float HP;
	float fullness;
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

	void update(float dt, Vector2 sheeppos);
	void render() const;
	void checkState();
	void handleState(Vector2 sheeppos);

	float speed;
	float max_speed;
	float roamweight;
	float seekweight;
	Vector2 roam();
	Vector2 seek(Vector2 target);

	float hunger;
	float detection_radius;
	bool nearSheep;
	bool hit;
	wolfState state;
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 denposition;
};

