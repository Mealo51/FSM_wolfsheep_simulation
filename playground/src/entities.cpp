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
	speed = 1.f * tile_len / 2;
	detection_radius = 4.f * tile_len;
	position = { (float)GetRandomValue(0 + static_cast<int>(sheep_radius), 1024 - static_cast<int>(sheep_radius)),
		(float)GetRandomValue(0 + static_cast<int>(sheep_radius), 1024 - static_cast<int>(sheep_radius)) };
	velocity = { 0.f,0.f };
	acceleration = { 0.f,0.f };
	state = sheepState::roaming;
}

void sheep::update(float dt)
{
	velocity = Vector2Add(velocity, acceleration * dt);
	position = Vector2Add(position, velocity);
	checkState();
	handleState();
	reproduce_cd--;
}

void sheep::render() const
{
	DrawCircleV(position, sheep_radius, WHITE);
	
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
		if(checkWolf()) {
			state = sheepState::fleeing;
		}
		else if (checkSheep()&&fullness >=80 && reproduce_cd <=0.f) {
			state = sheepState::reproducing;
		}
		else if (searchGrass()) {
			state = sheepState::eating;
		}
		else
		{
			acceleration = Random::rdirection() * speed; // random roaming direction
		}
		break;
	case sheepState::eating:
		eatGrass();
		break;
	case sheepState::fleeing:
		checkWolf();
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

bool sheep::checkSheep()
{
	return false;
}

bool sheep::checkWolf()
{
	return false;
}

bool sheep::searchGrass()
{
	return false;
}

void sheep::eatGrass()
{
}

sheep sheep::reproduce()
{
	reproduce_cd = 600.f; //reset reproduce cooldown
	HP -= 30.f;
	return sheep();
}

void sheep::defecate()
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