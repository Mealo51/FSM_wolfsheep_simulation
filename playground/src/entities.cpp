// entities.cpp

#include "entities.hpp"
#include "common.hpp"

constexpr Color NON_COLLIDING_COLOR = LIME;

sheep::sheep()
{
	HP = 100;
	fullness = 0;
	reproduce_cd = 20;
	detection_radius = 4;
	speed = 10;
	wolfNearby = false;
	position = { Random::rangef(0.f, 1024.f),Random::rangef(0.f, 1024.f) };
	velocity = { 0.f,0.f };
	acceleration = { 0.f,0.f };
}

void sheep::update(float dt)
{
	velocity = Vector2Add(velocity, acceleration * dt);
	position = Vector2Add(position, velocity);
}

void sheep::render() const
{
	DrawCircleV(position, 5, LIGHTGRAY);
}