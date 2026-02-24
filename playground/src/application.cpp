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

	m_sheep.reserve(10);
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
}

void App::render()
{
	for (auto& g : m_grass) {
		g.render();
	}
}
