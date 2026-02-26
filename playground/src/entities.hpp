// entities.hpp

#pragma once

#include "raylib.h"

struct App;

enum class sheepState {
	roaming,
	eating,
	fleeing,
	reproducing,
	full,
	dead
};

struct manure
{
	manure(Vector2 pos);
	Vector2 position;
	float lifetime;
	void render() const;
};

struct sheep {
	sheep();

	void update(float dt, App& app, Vector2 wolfpos, Vector2 sheeppos);
	void render() const;

	void sense(App& app);
	void decide();
	void act(float dt, Vector2 wolfpos, Vector2 sheeppos);
	float sensecd;
	float decidecd;

	sheep reproduce();
	void eatGrass();
	manure defecate();

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
	float defecate_cd;
	float eat_cd;
	float detection_radius;
	bool nearWolf;
	bool nearSheep;
	bool nearGrass;
	bool nearManure;
	bool isAlive;
	sheepState state;
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
};

enum class wolfState {
	roaming,
	attacking,
	sleeping,
	returning
};

struct wolf {
	wolf();

	void update(float dt, App& app);
	void render() const;

	void sense(App& app);
	void decide();
	void act(float dt, Vector2 sheeppos);
	float decidecd;
	float sensecd;

	float speed;
	float max_speed;
	float roamweight;
	float seekweight;
	float dragweight;
	Vector2 roam();
	Vector2 seek(Vector2 target);
	Vector2 drag();

	float hunger;
	float detection_radius;
	float hit_cd;
	bool nearSheep;
	bool hit;
	wolfState state;
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 denposition;
};

