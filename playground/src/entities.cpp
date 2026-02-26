// entities.cpp

#include "entities.hpp"
#include "common.hpp"
#include "Constant.hpp"

constexpr Color NON_COLLIDING_COLOR = LIME;

sheep::sheep()
{
	HP = 100.f * 60.f;
	fullness = 0.f;
	reproduce_cd = 600.f;  //10 sec cooldown at 60 fps, can be modified by fullness or other factors
	detection_radius = 3.f * tile_len;
	position = { (float)GetRandomValue(0 + static_cast<int>(sheep_radius), 1024 - static_cast<int>(sheep_radius)),
		(float)GetRandomValue(0 + static_cast<int>(sheep_radius), 1024 - static_cast<int>(sheep_radius)) };
	state = sheepState::roaming;
	nearWolf = false;
	nearManure = false;
	nearSheep = false;
	nearGrass = false;
	
	//movement
	velocity = { 0.f,0.f };
	acceleration = { 0.f,0.f };
	speed = 1.f * tile_len * 0.2f;
	min_speed = 20.f;
	max_speed = 150.f;
	fleeweight = 0.5f;
	roamweight = 1.f;
	dragweight = 0.1f;
}

void sheep::update(float dt, Vector2 wolfpos)
{
	velocity += acceleration * dt;
	position += velocity * dt;
	checkState();
	handleState();
	reproduce_cd--;
	acceleration += roam();
	acceleration += flee(wolfpos);
	acceleration += drag();
	//acceleration = Vector2Clamp(acceleration, Vector2{ min_speed, min_speed }, Vector2{ max_speed, max_speed });
}

Color debugColor = {255, 0, 0, 10};

void sheep::render() const
{
	DrawCircleV(position, sheep_radius, WHITE);
	DrawCircleV(position, detection_radius, debugColor); //debug detection radius
	//debug text
	switch (state)
	{
	case sheepState::roaming:
		DrawText("Roaming", static_cast<int>(position.x) - 20,
			static_cast<int>(position.y) - 30, 10, BLACK);
		break;
	case sheepState::eating:
		DrawText("Eating", static_cast<int>(position.x) - 20,
			static_cast<int>(position.y) - 30, 10, BLACK);
		break;
	case sheepState::fleeing:
		DrawText("Fleeing", static_cast<int>(position.x) - 20,
			static_cast<int>(position.y) - 30, 10, BLACK);
		break;
	case sheepState::reproducing:
		DrawText("Reproducing", static_cast<int>(position.x) - 30,
			static_cast<int>(position.y) - 30, 10, BLACK);
		break;
	case sheepState::defecating:
		DrawText("Defecating", static_cast<int>(position.x) - 30,
			static_cast<int>(position.y) - 30, 10, BLACK);
		break;
	}
}

void sheep::checkState()
{
	switch (state) {
	case sheepState::roaming:
		if (nearWolf) {
			state = sheepState::fleeing;
		}
		else if (nearSheep && fullness >= 80 && reproduce_cd <= 0.f) {
			state = sheepState::reproducing;
		}
		else if (nearGrass) {
			state = sheepState::eating;
		}
		break;
	case sheepState::eating:
		if (nearSheep && fullness >= 80 && reproduce_cd <= 0.f) {
			state = sheepState::reproducing;
		}
		else if (!nearWolf && !nearGrass)
		{
			state = sheepState::roaming;
		}
		break;
	case sheepState::fleeing:
		if (!nearWolf)
		{
			state = sheepState::roaming;
		}
		break;
	case sheepState::reproducing:
		break;
	case sheepState::defecating:
		break;
	}
}

void sheep::handleState()
{
	switch (state) {

	case sheepState::roaming:
		// Logic for roaming behavior
		break;
	case sheepState::eating:
		eatGrass();
		break;
	case sheepState::fleeing:
		break;
	case sheepState::reproducing:
		reproduce();
		break;
	case sheepState::defecating:
		defecate();
		break;
	}

}


//movement functions
Vector2 sheep::flee(Vector2 wolfPos)
{
	Vector2 toWolf = wolfPos - position;
	if (Vector2Length(toWolf) > detection_radius) {
		return { 0.f, 0.f }; // No fleeing if the wolf is outside the detection radius
	}
	auto away = Vector2Normalize(position - wolfPos);
	auto desired_velocity = away * speed;
	return (desired_velocity - velocity) * fleeweight;
}

Vector2 sheep::roam()
{
	return Vector2{ speed * static_cast<float>(GetRandomValue(-1, 1)),
		speed * static_cast<float>(GetRandomValue(-1, 1)) } *roamweight;
}

Vector2 sheep::drag()
{
	return -velocity * dragweight;
}

//action functions
void sheep::eatGrass()
{
	fullness += 25.f;
	HP += 20.f;
}

sheep sheep::reproduce()
{
	reproduce_cd = 600.f; //reset reproduce cooldown
	HP -= 30.f;
	return sheep();
}

void sheep::defecate()
{
	fullness = 20.f;
}

wolf::wolf()
{
	hunger = 0;
	speed = 1.5f * tile_len * 0.2f;
	detection_radius = 1.5f * tile_len ;
	denposition = { (float)GetRandomValue(0 + static_cast<int>(wolf_radius), 1024 - static_cast<int>(wolf_radius)),
		(float)GetRandomValue(0 + static_cast<int>(wolf_radius), 1024 - static_cast<int>(wolf_radius)) };
	position = denposition;
	velocity = { 0,0 };
	acceleration = { 0,0 };
	state = wolfState::sleeping;
}

void wolf::update(float dt)
{
	velocity = Vector2Add(velocity, acceleration * dt);
	position = Vector2Add(position, velocity);
	checkState();
	handleState();
}

void wolf::render() const
{
	DrawCircleV(position, wolf_radius, DARKGRAY);
	DrawCircleV(position, detection_radius, debugColor);
	//debug text
	DrawText(TextFormat("WHunger: %.1f", hunger), 10,
		40, 20, NON_COLLIDING_COLOR);
	switch (state)
	{
	case wolfState::roaming:
		DrawText("Roaming", static_cast<int>(position.x) - 20,
			static_cast<int>(position.y) - 30, 10, BLACK);
		break;
	case wolfState::sleeping:
		DrawText("Sleeping", static_cast<int>(position.x) - 20,
			static_cast<int>(position.y) - 30, 10, BLACK);
		break;
	case wolfState::attacking:
		DrawText("Attacking", static_cast<int>(position.x) - 20,
			static_cast<int>(position.y) - 30, 10, BLACK);
		break;
	}
}

void wolf::checkState()
{
	switch (state) {
	case wolfState::roaming:
		if (checkSheep()) {
			state = wolfState::attacking;
		}
		else if (hunger <= 20.f) {
			state = wolfState::sleeping;
		}
		break;
	case wolfState::sleeping:
		if (hunger >= 60.f) {
			state = wolfState::roaming;
		}
		break;
	case wolfState::attacking:
		if (hunger <= 20.f) {
			state = wolfState::sleeping;
		}
		else if (hunger >= 20.f) {
			state = wolfState::roaming;
		}
		break;
	}
}

void wolf::handleState()
{
	switch (state) {
	case wolfState::roaming:
		hunger += 5.f / 60.f; // increase hunger over time
		break;
	case wolfState::attacking:
		if (attack()) {
			hunger -= 60.f;
		}
		break;
	case wolfState::sleeping:
		hunger += 5.f / 60.f;
		break;
	}
}

bool wolf::checkSheep()
{
	return false;
}

bool wolf::attack()
{
	return false;
}