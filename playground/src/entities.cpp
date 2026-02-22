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
	velocity = { 0,0 };
	acceleration = { 0,0 };
}

