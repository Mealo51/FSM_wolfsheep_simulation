// entities.cpp

#include "entities.hpp"
#include "common.hpp"
#include "Constant.hpp"
#include "application.hpp"
#include "collision.hpp"

constexpr Color NON_COLLIDING_COLOR = LIGHTGRAY;

manure::manure(Vector2 pos)
{
	position = pos;
	lifetime = 180.f;
}

void manure::render() const
{
	DrawCircleV(position, 5.f, BROWN);
}

sheep::sheep()
{
	HP = 100.f;
	fullness = 0.f;
	eat_cd = 0.f;
	reproduce_cd = 600.f;
	defecate_cd = 600.f;
	detection_radius = 3.f * tile_len;
	position = { (float)GetRandomValue(0 + static_cast<int>(sheep_radius), 1024 - static_cast<int>(sheep_radius)),
		(float)GetRandomValue(0 + static_cast<int>(sheep_radius), 1024 - static_cast<int>(sheep_radius)) };


	state = sheepState::roaming;
	decidecd = 0.f;
	sensecd = 0.f;
	nearWolf = false;
	nearManure = false;
	nearSheep = false;
	nearGrass = false;
	isAlive = true;

	//movement
	velocity = { 0.f,0.f };
	acceleration = { 0.f,0.f };
	speed = 1.f * tile_len * 0.2f;
	max_speed = 1.5f * tile_len * 0.2f;
	fleeweight = 2.f;
	roamweight = 1.2f;
	dragweight = 0.1f;
	cohesionweight = 1.f;
}

void sheep::update(float dt, App& app, Vector2 wolfpos, Vector2 sheeppos)
{
	reproduce_cd -= tick_rate * dt;
	defecate_cd -= tick_rate * dt;
	eat_cd += tick_rate * dt;
	sensecd += tick_rate * dt;
	decidecd += tick_rate * dt;

	if (sensecd >= 0.25f) {
		sense(app);
		sensecd = 0.0f;
	}

	if (decidecd >= 0.5f) {
		decide();
		decidecd = 0.0f;
	}

	act(dt, wolfpos, sheeppos);
}

Color debugColor = { 255, 0, 0, 10 };

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
		DrawText("Full", static_cast<int>(position.x) - 30,
			static_cast<int>(position.y) - 30, 10, BLACK);
		break;
	}
}

void sheep::sense(App& app) {
	nearWolf = false;
	nearSheep = false;
	nearGrass = false;

	if (Collision::checkSheepWolf(*this, app.m_wolf)) {
		nearWolf = true;
	}

	for (auto& g : app.m_grass) {
		if (Collision::checkSheepGrass(*this, g)) {
			nearGrass = true;
		}
	}

	for (auto& other : app.m_sheep) {
		if (this != &other && Collision::checkSheepSheep(*this, other)) {
			nearSheep = true;
		}
	}

	for (auto& m : app.m_manure) {
		if (Collision::checkSheepManure(*this, m)) {
			nearManure = true;
		}
	}
}

void sheep::decide() {
	// This function looks at the booleans set by sense() and updates 'state'
	// This is where your state machine logic lives.
	switch (state) {
	case sheepState::roaming:
		if (nearWolf) state = sheepState::fleeing;
		else if (nearGrass && eat_cd >= 300.0f && !nearManure) state = sheepState::eating;
		break;
	case sheepState::eating:
		if (nearWolf) state = sheepState::fleeing;
		else if (!nearGrass) state = sheepState::roaming;
		else if (fullness >= 100.f && reproduce_cd <= 0.f) state = sheepState::reproducing;
		else if (fullness >= 80.f) state = sheepState::full;
		break;
	case sheepState::fleeing:
		if (!nearWolf) state = sheepState::roaming;
		break;
	case sheepState::reproducing:
		if (reproduce_cd > 0.f) state = sheepState::roaming;
		break;
	case sheepState::full:
		if (fullness <= 20.f || nearManure) state = sheepState::roaming;
		break;
	}
}

void sheep::act(float dt, Vector2 wolfpos, Vector2 sheeppos) {
	acceleration = drag();

	switch (state) {
	case sheepState::roaming:
		acceleration += roam();
		break;
	case sheepState::eating:
		eatGrass();
		eat_cd = 0.f; // reset eat cooldown
		break;
	case sheepState::fleeing:
		acceleration += flee(wolfpos);
		break;
	case sheepState::reproducing:
		acceleration += cohesion(sheeppos);
		break;
	case sheepState::full:
		if(defecate_cd <= 0.f) {
			defecate_cd = 600.f; //reset defecate cooldown
			defecate();
			fullness = 20.f;
		}
		break;
	}

	//movement
	velocity += acceleration * dt;
	velocity = Vector2Clamp(velocity, { -max_speed, -max_speed }, { max_speed, max_speed });
	position += velocity * dt;
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

manure sheep::defecate()
{
	return manure(position);
}

wolf::wolf()
{
	hunger = 0;
	detection_radius = 1.5f * tile_len;
	denposition = { (float)GetRandomValue(0 + static_cast<int>(wolf_radius), 1024 - static_cast<int>(wolf_radius)),
		(float)GetRandomValue(0 + static_cast<int>(wolf_radius), 1024 - static_cast<int>(wolf_radius)) };
	position = denposition;

	state = wolfState::sleeping;
	sensecd = 0.f;
	decidecd = 0.f;
	nearSheep = false;
	hit = false;

	speed = 1.1f * tile_len * 0.2f;
	max_speed = 1.5f * tile_len * 0.2f;
	velocity = { 0,0 };
	acceleration = { 0,0 };
	seekweight = 1.5f;
	roamweight = 0.6f;

}

void wolf::update(float dt, App& app)
{
	sensecd += tick_rate * dt;
	decidecd += tick_rate * dt;

	if (sensecd >= 0.2f) {
		sense(app);
		sensecd = 0.0f;
	}


	if (decidecd >= 0.4f) {
		decide();
		decidecd = 0.0f;
	}

	Vector2 target = (app.m_sheep.empty()) ? denposition : app.m_sheep[0].position;
	act(dt, target);
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

void wolf::sense(App& app) 
{
	nearSheep = false;
	hit = false;
	for (const auto& s : app.m_sheep) 
	{
		if (Collision::checkWolfSheep(*this, s)) 
		{
			nearSheep = true;
			break; 
		}
	}
}

void wolf::decide() {
	switch (state) {
	case wolfState::sleeping:
		if (hunger >= 60.f) state = wolfState::roaming; // Wake up when hungry
		break;

	case wolfState::roaming:
		if (nearSheep) state = wolfState::attacking;
		else if (hunger <= 10.f) state = wolfState::sleeping; // Go back to den
		break;

	case wolfState::attacking:
		if (hunger <= 20.f || !nearSheep) state = wolfState::roaming;
		break;
	}
}

void wolf::act(float dt, Vector2 targetPos) {
	acceleration = { 0, 0 };

	switch (state) {
	case wolfState::sleeping:
		hunger += 2.0f * dt;
		velocity = { 0, 0 };
		position = denposition;
		break;

	case wolfState::roaming:
		hunger += 5.0f * dt;
		acceleration += roam();
		break;

	case wolfState::attacking:
		hunger += 8.0f * dt;
		acceleration += seek(targetPos);

		if (hit) { 
			hunger = fmaxf(0.f, hunger - 60.f);
			hit = false;
		}
		break;
	}

	velocity += acceleration * dt;
	velocity = Vector2Clamp(velocity, { -max_speed, -max_speed }, { max_speed, max_speed });
	position += velocity * dt;
}

Vector2 wolf::roam()
{
	return Vector2{ speed * static_cast<float>(GetRandomValue(-1, 1)),
		speed * static_cast<float>(GetRandomValue(-1, 1)) } *roamweight;
}

Vector2 wolf::seek(Vector2 target)
{
	Vector2 toTarget = target - position;
	auto desired_velocity = Vector2Normalize(toTarget) * speed;
	return (desired_velocity - velocity) * seekweight;
}
