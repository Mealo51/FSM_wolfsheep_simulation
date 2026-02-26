// application.cpp

#include "application.hpp"
#include "common.hpp"
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
		for(auto& s2 : app.m_sheep) {
			if (&s != &s2 && Collision::checkSheepSheep(s, s2)) {
				s.nearSheep = true;
				s2.nearSheep = true;
			}
		}
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
	}
	for(auto& s : m_sheep) {
		s.update(dt);
	}
	m_wolf.update(dt);
}

void App::render()
{
	for (auto& g : m_grass) {
		g.render();
	}
	for(auto& s : m_sheep) {
		s.render();
	}
	m_wolf.render();
}