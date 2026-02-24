// entities.cpp

#include "entities.hpp"
#include "common.hpp"
#include "Constant.hpp"

constexpr Color NON_COLLIDING_COLOR = LIME;

sheep::sheep()
{
	HP = 100.f;
	fullness = 0.f;
	reproduce_cd = 20.f;
	speed = 1.f * tile_len;
	detection_radius = 4.f * tile_len;
	wolfNearby = false;
	position = { (float)GetRandomValue(0 + static_cast<int>(sheep_radius), 1024 - static_cast<int>(sheep_radius)), 
		(float)GetRandomValue(0 + static_cast<int>(sheep_radius), 1024 - static_cast<int>(sheep_radius)) };
	velocity = { 0.f,0.f };
	acceleration = { 0.f,0.f };
}

void sheep::update(float dt)
{
	velocity = Vector2Add(velocity, acceleration * dt);
	position = Vector2Add(position, velocity);
	sense();
}

void sheep::render() const
{
	DrawCircleV(position, sheep_radius, WHITE);
}

void sheep::sense()
{
	checkSheep();
	checkWolf();
	searchGrass();
}

void sheep::checkSheep()
{
}

void sheep::checkWolf()
{
}

void sheep::searchGrass()
{
}


wolf::wolf()
{
	hunger = 0;
	speed = 1.5f * tile_len;
	detection_radius = 2.f * tile_len;
	denposition = { (float)GetRandomValue(0 + static_cast<int>(wolf_radius), 1024 - static_cast<int>(wolf_radius)), 
		(float)GetRandomValue(0 + static_cast<int>(wolf_radius), 1024 - static_cast<int>(wolf_radius)) };
	position = denposition;
	velocity = { 0,0 };
	acceleration = { 0,0 };
}

void wolf::update(float dt)
{
	velocity = Vector2Add(velocity, acceleration * dt);
	position = Vector2Add(position, velocity);
	sense();
}

void wolf::render() const
{
	DrawCircleV(position, wolf_radius, DARKGRAY);
}

void wolf::sense()
{
	checkSheep();
}

void wolf::checkSheep()
{
}