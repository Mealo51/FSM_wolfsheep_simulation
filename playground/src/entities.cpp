// entities.cpp

#include "entities.hpp"
#include "common.hpp"
#include "Constant.hpp"
#include "application.hpp"
#include "collision.hpp"
#include "grass.hpp"

constexpr Color NON_COLLIDING_COLOR = LIGHTGRAY;

//manure
manure::manure(Vector2 pos)
{
	position = pos;
	lifetime = 180.f;
}

void manure::render() const
{
	DrawCircleV(position, 5.f, BROWN);
}

//sheep
sheep::sheep()
{
	HP = 100.f;
	fullness = 0.f;
	eat_cd = 0.f;
	reproduce_cd = 30.f;
	defecate_cd = 0.f;
	detection_radius = 3.f * tile_len;
	position = { (float)GetRandomValue(0 + static_cast<int>(sheep_radius), 1024 - static_cast<int>(sheep_radius)),
		(float)GetRandomValue(0 + static_cast<int>(sheep_radius), 1024 - static_cast<int>(sheep_radius)) };
	mateposition = { 0,0 };

	state = sheepState::roaming;
	decidecd = 0.f;
	sensecd = 0.f;
	nearWolf = false;
	nearManure = false;
	nearSheep = false;
	canMate = false;
	nearGrass = false;
	eating = false;
	isAlive = true;
	targetGrass = nullptr;

	//movement
	velocity = { 0.f,0.f };
	acceleration = { 0.f,0.f };
	speed = 1.f * tile_len;
	max_speed = 1.5f * tile_len;
	fleeweight = 3.f;
	roamweight = 1.8f;
	dragweight = 0.1f;
	cohesionweight = 2.f;
	avoidmanureweight = 2.f;
	avoidwallsweight = 3.f;
}

void sheep::update(float dt, App& app, Vector2 wolfpos)
{
	reproduce_cd -= dt;
	defecate_cd -= dt;
	eat_cd += dt;
	sensecd += dt;
	decidecd += dt;

	if (sensecd >= 0.25f) {
		sense(app);
		sensecd = 0.0f;
	}

	if (decidecd >= 0.5f) {
		decide();
		decidecd = 0.0f;
	}

	act(dt, app, wolfpos);
}

Color debugColor = { 255, 0, 0, 10 };

void sheep::render() const
{
	DrawCircleV(position, sheep_radius, WHITE);
	//debug drawing
	DrawCircleV(position, detection_radius, debugColor); //detection radius
	DrawText(TextFormat("fullness: %.1f", fullness), static_cast<int>(position.x) - 30,
		static_cast<int>(position.y) - 40, 10, BLACK);

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
	eating = false;

	if (Collision::searchSheepWolf(*this, app.m_wolf)) {
		nearWolf = true;
	}
	if (Collision::checkSheepWolf(*this, app.m_wolf)) {
		isAlive = false;
	}

	for (auto& g : app.m_grass) {
		// CHECK: Is the grass actually GROWN?
		if (g.state == GrassState::grown) {
			if (Collision::checkSheepGrass(*this, g)) {
				nearGrass = true;
				// Optional: Store a pointer to this specific grass 
				// so the sheep knows WHICH one to turn back to dirt
				targetGrass = &g;
				break; // Found food, stop looking
			}
		}
	}

	for (auto& other : app.m_sheep) {
		if (this != &other && Collision::checkSheepSheep(*this, other)) {
			nearSheep = true;

			if (HP >= 60.f && reproduce_cd <= 0.f && other.HP >= 60.f && other.reproduce_cd <= 0.f) {
				canMate = true;
				mateposition = other.position;
			}
		}
	}

		for (auto& m : app.m_manure) {
			if (Collision::checkSheepManure(*this, m)) {
				nearManure = true;
			}
		}

		if (Collision::checkSheepWindow(*this, app.bounds)) {
			position = Vector2Clamp(position, Vector2{ sheep_radius, sheep_radius },
				Vector2{ app.bounds.x - sheep_radius, app.bounds.y - sheep_radius });
		}
}

void sheep::decide() {
	if (!isAlive) {
		state = sheepState::dead;
		return;
	}
	switch (state) {
	case sheepState::roaming:
		if (nearWolf) state = sheepState::fleeing;
		else if (nearGrass && !nearManure && eat_cd >= 5.0f) state = sheepState::eating;
		else if (canMate) state = sheepState::reproducing;
		break;
	case sheepState::eating:
		if (nearWolf) state = sheepState::fleeing;
		else if (!nearGrass) state = sheepState::roaming;
		else if (targetGrass == nullptr || targetGrass->state != GrassState::grown) {
			state = sheepState::roaming;
		}
		else if (fullness >= 100.f) state = sheepState::full;
		else if (canMate) state = sheepState::reproducing;
		break;
	case sheepState::fleeing:
		if (!nearWolf) state = sheepState::roaming;
		break;
	case sheepState::reproducing:
		if (nearWolf) state = sheepState::fleeing;
		else if (reproduce_cd > 0.f) state = sheepState::roaming;
		break;
	case sheepState::full:
		if (fullness <= 20.f || nearManure) state = sheepState::roaming;
		break;
	}
}

void sheep::act(float dt, App& app, Vector2 wolfpos) {
	acceleration = drag();

	switch (state) {
	case sheepState::roaming:
		acceleration += roam();
		acceleration += cohesion();
		for (auto& m : app.m_manure) {
			acceleration += avoidmanure(m.position);
		}
		acceleration += avoidWalls();
		break;
	case sheepState::eating:
		velocity = { 0.f, 0.f }; // stop moving while eating
		fullness += 15.f * dt;
		HP += 5.f * dt;
		eating = false;
		// Once the sheep is full enough, the grass is "eaten"
		if (fullness >= 100.f) {
			eat_cd = 0.f; // reset eat cooldown
			if (targetGrass != nullptr) {
				targetGrass->state = GrassState::dirt; // Grass is gone!
				targetGrass->grow_progress = 0.f;       // Reset growth timer
			}
			state = sheepState::full;
		}
		break;
	case sheepState::fleeing:
		acceleration += flee(wolfpos);
		acceleration += avoidWalls();
		break;
	case sheepState::reproducing:
		acceleration += cohesion();
		break;
	case sheepState::full:
		if (defecate_cd <= 0.f) {
			defecate_cd = 10.f; //reset defecate cooldown
			app.m_manure.emplace_back(defecate());
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
	auto away = Vector2Normalize(position - wolfPos);
	auto desired_velocity = away * speed;
	return (desired_velocity - velocity) * fleeweight;
}

Vector2 sheep::roam()
{
	float rx = static_cast<float>(GetRandomValue(-100, 100)) / 100.0f;
	float ry = static_cast<float>(GetRandomValue(-100, 100)) / 100.0f;

	Vector2 drift = Vector2Normalize({ rx, ry });
	return drift * speed * roamweight;
}

Vector2 sheep::drag()
{
	return -velocity * dragweight;
}

Vector2 sheep::cohesion()
{
	Vector2 toSheep = mateposition - position;
	if (Vector2Length(toSheep) > detection_radius) {
		return { 0.f, 0.f }; // No cohesion if the other sheep is outside the detection radius
	}
	auto desired_velocity = Vector2Normalize(toSheep) * speed;
	return (desired_velocity - velocity) * cohesionweight;
}

Vector2 sheep::avoidmanure(Vector2 manurePos)
{
	Vector2 toManure = manurePos - position;
	if (Vector2Length(toManure) > detection_radius) {
		return { 0.f, 0.f }; // No avoidance if the manure is outside the detection radius
	}
	auto away = Vector2Normalize(position - manurePos);
	auto desired_velocity = away * speed;
	return (desired_velocity - velocity) * avoidmanureweight;
}

Vector2 sheep::avoidWalls()
{
	float margin = 50.0f; // Distance from wall to start turning
	Vector2 desired = { 0, 0 };

	// If too far left, desire to go right
	if (position.x < margin) desired.x = speed;
	// If too far right, desire to go left
	else if (position.x > 1024 - margin) desired.x = -speed;

	// If too far top, desire to go down
	if (position.y < margin) desired.y = speed;
	// If too far bottom, desire to go up
	else if (position.y > 1024 - margin) desired.y = -speed;

	if (desired.x != 0 || desired.y != 0) {
		desired = Vector2Normalize(desired) * speed;
		return (desired - velocity) * avoidwallsweight; // Return the steering force
	}

	return { 0, 0 };
}

//action functions
sheep sheep::reproduce()
{
	reproduce_cd = 30.f; //reset reproduce cooldown
	HP = 20.f;
	sheep offspring;
	offspring.position = position; // Start offspring at parent's position
	return offspring;
}

manure sheep::defecate()
{
	return manure(position);
}


//wolf
wolf::wolf()
{
	hunger = 0;
	detection_radius = 5.f * tile_len;
	denposition = { (float)GetRandomValue(0 + static_cast<int>(wolf_radius), 1024 - static_cast<int>(wolf_radius)),
		(float)GetRandomValue(0 + static_cast<int>(wolf_radius), 1024 - static_cast<int>(wolf_radius)) };
	position = denposition;

	state = wolfState::sleeping;
	sensecd = 0.f;
	decidecd = 0.f;
	nearSheep = false;
	hit = false;
	hit_cd = 0.f;
	targetsheeppos = { 0,0 };

	speed = 1.5f * tile_len;
	max_speed = 4.f * tile_len;
	velocity = { 0,0 };
	acceleration = { 0,0 };
	seekweight = 2.f;
	roamweight = 3.f;
	dragweight = 0.1f;
	avoidwallsweight = 5.f;
}

void wolf::update(float dt, App& app)
{
	sensecd += dt;
	decidecd += dt;
	hit_cd += dt;

	if (hunger >= 100.f) {
		hunger = 100.f;
	}

	if (sensecd >= 0.25f) {
		sense(app);
		sensecd = 0.0f;
	}


	if (decidecd >= 0.5f) {
		decide();
		decidecd = 0.0f;
	}

	Vector2 currentMoveTarget = (nearSheep) ? targetsheeppos : denposition;
	act(dt, currentMoveTarget);
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
	case wolfState::returning:
		DrawText("Returning", static_cast<int>(position.x) - 20,
			static_cast<int>(position.y) - 30, 10, BLACK);
		break;
	}
}

void wolf::sense(App& app)
{
	nearSheep = false;
	hit = false;
	float closestDist = detection_radius;

	for (auto& s : app.m_sheep)
	{
		float dist = Vector2Distance(position, s.position);
		if (!s.isAlive) continue;
		// Find closest sheep within radius
		if (dist < closestDist) {
			closestDist = dist;
			targetsheeppos = s.position;
			nearSheep = true;
		}
		if (Collision::checkWolfSheep(*this, s))
		{
			if (hit_cd >= 10.0f) {
				hit = true;
				hit_cd = 0.f;
				s.isAlive = false;
			}
		}
	}
	if (Collision::checkWolfWindow(*this, app.bounds))
	{
		position = Vector2Clamp(position, Vector2{ wolf_radius, wolf_radius },
			Vector2{ app.bounds.x - wolf_radius, app.bounds.y - wolf_radius });
	}
}

void wolf::decide() {
	switch (state) {
	case wolfState::sleeping:
		if (hunger >= 40.f) state = wolfState::roaming; // Wake up when hungry
		break;

	case wolfState::roaming:
		if (nearSheep) state = wolfState::attacking;
		else if (hunger <= 20.f) state = wolfState::returning; // Go back to den
		break;

	case wolfState::attacking:
		if (!nearSheep) state = wolfState::roaming;
		else if (hunger <= 20.f) state = wolfState::returning;
		break;
	}
}

void wolf::act(float dt, Vector2 targetPos) {
	acceleration = { 0,0 };
	acceleration += drag();

	if (hit) {
		hunger -= 100.f;
		if (hunger < 0) hunger = 0; // Clamp to 0
		hit = false; // Reset the flag
	}

	switch (state) {
	case wolfState::sleeping:
		hunger += 10.0f * dt;
		velocity = { 0, 0 };
		position = denposition;
		break;
	case wolfState::roaming:
		hunger += 20.0f * dt;
		acceleration += roam();
		acceleration += avoidWalls();
		break;
	case wolfState::attacking:
		hunger += 20.0f * dt;
		acceleration += seek(targetPos);
		break;
	case wolfState::returning:
		acceleration += seek(denposition);
		if (Vector2Distance(position, denposition) < 5.f) {
			state = wolfState::sleeping;
			position = denposition;
			velocity = { 0, 0 };
		}
		break;
	}

	velocity += acceleration * dt;
	velocity = Vector2Clamp(velocity, { -max_speed, -max_speed }, { max_speed, max_speed });
	position += velocity * dt;
}

Vector2 wolf::roam()
{
	float rx = (float)GetRandomValue(-100, 100) / 100.0f;
	float ry = (float)GetRandomValue(-100, 100) / 100.0f;

	return Vector2Normalize({ rx, ry }) * speed * roamweight;
}

Vector2 wolf::seek(Vector2 target)
{
	Vector2 toTarget = target - position;
	auto desired_velocity = Vector2Normalize(toTarget) * speed;
	return (desired_velocity - velocity) * seekweight;
}

Vector2 wolf::drag()
{
	return -velocity * dragweight;
}

Vector2 wolf::avoidWalls()
{
	float margin = 50.0f; // Distance from wall to start turning
	Vector2 desired = { 0, 0 };

	// If too far left, desire to go right
	if (position.x < margin) desired.x = speed;
	// If too far right, desire to go left
	else if (position.x > 1024 - margin) desired.x = -speed;

	// If too far top, desire to go down
	if (position.y < margin) desired.y = speed;
	// If too far bottom, desire to go up
	else if (position.y > 1024 - margin) desired.y = -speed;

	if (desired.x != 0 || desired.y != 0) {
		desired = Vector2Normalize(desired) * speed;
		return (desired - velocity) * avoidwallsweight; // Return the steering force
	}

	return { 0, 0 };
}