// entities.hpp

#pragma once

#include "raylib.h"

struct sheep {
	sheep() = default;

	void update(float dt);
	void render() const;
	bool checkWolf(wolf w);
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


struct Circle {
	Circle() = default;

	void update(float dt);
	void render() const;

	Vector2 position{ 0,0 };
	float   radius{ 0 };
	Vector2 velocity{ 0,0 };
	Color   color{ 0 };
};

struct AlignedRectangle {
	AlignedRectangle() = default;

	void update(float dt);
	void render() const;

	Vector2 position{ 0,0 };
	Vector2 extent{ 0,0 };
	Color   color{ 0 };
};

struct RotatedRectangle {
	RotatedRectangle() = default;

	void update(float dt);
	void render() const;

	Vector2 position{ 0,0 };
	Vector2 extent{ 0,0 };
	float   rotation{ 0 };
	Color   color{ 0 };
};
