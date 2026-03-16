// entities.hpp

#pragma once

#include "raylib.h"
#include <vector>

struct App;
struct grass;

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

	void update(float dt, App& app, Vector2 wolfpos);
	void render() const;

	void sense(App& app);
	void decide();
	void act(float dt, App& app, Vector2 wolfpos);
	float sensecd;
	float decidecd;

	sheep reproduce();
	manure defecate();

	Vector2 flee(Vector2 wolfPos);
	Vector2 roam();
	Vector2 drag();
	Vector2 cohesion();
	Vector2 avoidmanure(Vector2 manurePos);
	Vector2 avoidBorders();
	Vector2 avoidWalls(Vector2 pos);
	float fleeweight;
	float roamweight;
	float dragweight;
	float cohesionweight;
	float avoidmanureweight;
	float avoidborderweight;
	float avoidwallsweight;
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
	bool canMate;
	bool nearGrass;
	bool eating;
	bool nearManure;
	bool isAlive;
	grass* targetGrass;
	sheepState state;
	Vector2 mateposition;
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;

	std::vector<Vector2> path;
	float pathrefreshcd;
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
	void act(float dt, App& app);
	float decidecd;
	float sensecd;

	float speed;
	float max_speed;
	float roamweight;
	float seekweight;
	float dragweight;
	float avoidborderweight;
	float avoidwallsweight;
	Vector2 roam();
	Vector2 seek(Vector2 target);
	Vector2 drag();
	Vector2 avoidBorders();
	Vector2 avoidWalls(Vector2 pos);

	float hunger;
	float detection_radius;
	float hit_cd;
	bool nearSheep;
	bool hit;
	wolfState state;
	Vector2 position;
	Vector2	targetsheeppos;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 denposition;

	std::vector<Vector2> path;
	float pathrefreshcd;
};

