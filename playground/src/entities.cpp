// entities.cpp

#include "entities.hpp"
#include "common.hpp"
#include "Constant.hpp"

constexpr Color NON_COLLIDING_COLOR = LIME;

manure::manure(Vector2 pos)
{
	position = pos;
}

void manure::render() const
{
	DrawCircleV(position, 5.f, BROWN);
}

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
	max_speed = 1.5f * tile_len * 0.2f;
	fleeweight = 0.5f;
	roamweight = 1.f;
	dragweight = 0.1f;
	cohesionweight = 0.6f;
}

void sheep::update(float dt, Vector2 wolfpos, Vector2 sheeppos)
{
	velocity += acceleration * dt;
	position += velocity * dt;
	checkState();
	handleState(wolfpos, sheeppos);
	reproduce_cd--;
	acceleration += drag();
	velocity = Vector2Clamp(velocity, Vector2{ -max_speed, -max_speed }, Vector2{ max_speed, max_speed });
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
	case sheepState::full:
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
		if (!nearSheep || fullness < 80 || reproduce_cd > 0.f) {
			state = sheepState::roaming;
		}
		break;
	case sheepState::full:
		break;
	}
}

void sheep::handleState(Vector2 wolfpos, Vector2 sheeppos)
{
	switch (state) {
	case sheepState::roaming:
		acceleration += roam();
		break;
	case sheepState::eating:
		eatGrass();
		break;
	case sheepState::fleeing:
		acceleration += flee(wolfpos);
		break;
	case sheepState::reproducing:
		acceleration += cohesion(sheeppos);
		reproduce();
		break;
	case sheepState::full:
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

Vector2 sheep::cohesion(Vector2 sheeppos)
{
	Vector2 toSheep = sheeppos - position;
	if (Vector2Length(toSheep) > detection_radius) {
		return { 0.f, 0.f }; // No cohesion if the other sheep is outside the detection radius
	}
	auto desired_velocity = Vector2Normalize(toSheep) * speed;
	return (desired_velocity - velocity) * cohesionweight;
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
	detection_radius = 1.5f * tile_len ;
	denposition = { (float)GetRandomValue(0 + static_cast<int>(wolf_radius), 1024 - static_cast<int>(wolf_radius)),
		(float)GetRandomValue(0 + static_cast<int>(wolf_radius), 1024 - static_cast<int>(wolf_radius)) };
	position = denposition;
	state = wolfState::sleeping;
	nearSheep = false;
	hit = false;
	speed = 1.1f * tile_len * 0.2f;
	max_speed = 1.5f * tile_len * 0.2f;
	velocity = { 0,0 };
	acceleration = { 0,0 };
	seekweight = 0.5f;
	roamweight = 1.f;

}

void wolf::update(float dt, Vector2 sheeppos)
{
	velocity +=  acceleration * dt;
	velocity = Vector2Clamp(velocity, Vector2{ -max_speed, -max_speed }, Vector2{ max_speed, max_speed });
	position += velocity;
	checkState();
	handleState(sheeppos);
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
		if (nearSheep) {
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

void wolf::handleState(Vector2 sheeppos)
{
	switch (state) {
	case wolfState::roaming:
		hunger += 5.f / 60.f; // increase hunger over time
		acceleration += roam();
		break;
	case wolfState::attacking:
		acceleration += seek(sheeppos);
		if (hit) {
			hunger -= 60.f;
		}
		break;
	case wolfState::sleeping:
		hunger += 2.f / 60.f;
		break;
	}
}

Vector2 wolf::roam()
{
	return Vector2{ speed * static_cast<float>(GetRandomValue(-1, 1)),
		speed * static_cast<float>(GetRandomValue(-1, 1)) } * roamweight;
}

Vector2 wolf::seek(Vector2 target)
{
	Vector2 toTarget = target - position;
	if (Vector2Length(toTarget) > detection_radius) {
		return { 0.f, 0.f }; // No seeking if the target is outside the detection radius
	}
	auto desired_velocity = Vector2Normalize(toTarget) * speed;
	return (desired_velocity - velocity) * seekweight;
}
