// application.cpp

#include "application.hpp"
#include "common.hpp"
#include "Constant.hpp"
#include "collision.hpp"
#include <vector>

App::App(int width, int height)
{
	// note: screen bounds
	bounds = Vector2{ (float)width, (float)height };

	gx = 0.f;
	gy = 0.f;

	//sheep initialization
	m_sheep.reserve(10);
	for (int i = 0; i < 10; i++)
	{
		m_sheep.emplace_back(sheep());
	}

	//grass initialization
	m_grass.reserve(static_cast<int>(tile_x * tile_y));
	for (int i = 0; i < static_cast<int>(tile_x * tile_y); i++)
	{
		m_grass.emplace_back(grass(Vector2{ gx, gy }));
		gx += tile_len;
		if (gx >= bounds.x) {
			gx = 0.f;
			gy += tile_len;
		}
	}
	m_manure.reserve(10);
}

grass App::spread()
{
	int index = GetRandomValue(0, static_cast<int>(m_grass.size() - 1));
	for (auto& g : m_grass) {
		if (g.state == GrassState::grown && g.spread_attempts < 3) {
			for (int i = 0; i < 2; i++) {
				if (m_grass[index].state == GrassState::dirt) {
					m_grass[index] = grass(m_grass[index].position);
					g.spread_attempts++;
					g.spread_indices[i] = index;
				}
			}
		}
	}
	return grass(m_grass[index].position);
}

static void check_collisions(App& app)
{
	for (auto& s : app.m_sheep)
	{
		if (Collision::checkSheepWindow(s, app.bounds)) {
			//keeps the sheep inside window but current roam would result in sheep getting stuck at the edge of the window, 
			//need to implement better roaming behavior
			s.position = Vector2Clamp(s.position, Vector2{ sheep_radius, sheep_radius },
				Vector2{ app.bounds.x - sheep_radius, app.bounds.y - sheep_radius });
		}
		for (auto& s2 : app.m_sheep) {
			if (&s != &s2 && Collision::checkSheepSheep(s, s2)) {
				s.nearSheep = true;
				s2.nearSheep = true;
				s.position = Vector2Clamp(s.position, Vector2{ sheep_radius, sheep_radius },
					Vector2{ app.bounds.x - sheep_radius, app.bounds.y - sheep_radius });
				s2.position = Vector2Clamp(s2.position, Vector2{ sheep_radius, sheep_radius },
					Vector2{ app.bounds.x - sheep_radius, app.bounds.y - sheep_radius });
			}
		}
		s.nearSheep = false;
		for (auto& g : app.m_grass) {
			if (Collision::checkSheepGrass(s, g)) {
				s.nearGrass = true;
				if (g.state == GrassState::grown) {
					s.eatGrass();
					g.state = GrassState::dirt;
				}
			}
		}
		s.nearGrass = false;

		if (Collision::checkSheepWolf(s, app.m_wolf)) {
			s.nearWolf = true;
		}
		s.nearWolf = false;
		//wolf collision
		if (Collision::checkWolfSheep(app.m_wolf, s)) {
			app.m_wolf.nearSheep = true;
		}
		else {
			app.m_wolf.nearSheep = false;
		}
	}

	if (Collision::checkWolfWindow(app.m_wolf, app.bounds)) {
		app.m_wolf.position = Vector2Clamp(app.m_wolf.position, Vector2{ wolf_radius, wolf_radius },
			Vector2{ app.bounds.x - wolf_radius, app.bounds.y - wolf_radius });
	}
}

void App::update(float dt)
{
	m_input.update();
	constexpr float SPEED = 100.0f;
	Vector2 direction = m_input.value("move");
	check_collisions(*this);
	for (auto& g : m_grass) {
		g.update(dt);
		spread();
	}
	for (auto& s : m_sheep) {
		for (auto& s2 : m_sheep) {
			s.update(dt, m_wolf.position, s2.position);
		}
		if(s.state == sheepState::full) {
			m_manure.emplace_back(s.defecate());
		}
		if(s.state == sheepState::reproducing) {
			m_sheep.emplace_back(s.reproduce());
		}
	}
	for(auto& m : m_manure) {
		m.lifetime -= tick_rate * dt;
		if(m.lifetime <= 0) {
			m = m_manure.back();
			m_manure.pop_back();
		}
	}
	m_wolf.update(dt, m_sheep.empty() ? Vector2{ 0.f, 0.f } : m_sheep[0].position);
}

void App::render()
{
	for (auto& g : m_grass) {
		g.render();
	}
	for (auto& s : m_sheep) {
		s.render();
	}
	for(auto& m : m_manure) {
		m.render();
	}
	m_wolf.render();
}