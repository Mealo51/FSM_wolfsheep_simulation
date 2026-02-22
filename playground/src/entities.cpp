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
	DrawCircleV(position, sheep_radius, WHITE);
}

wolf::wolf()
{
	hunger = 0;
	speed = 1.5f * tile_len;
	detection_radius = 2.f * tile_len;
	position = { 0,0 };
	velocity = { 0,0 };
	acceleration = { 0,0 };
}