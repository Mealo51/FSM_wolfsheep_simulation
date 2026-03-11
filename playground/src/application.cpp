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
	const int sheep_count = 6;
	m_sheep.reserve(sheep_count);
	for (int i = 0; i < sheep_count; i++)
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
					m_grass[index].state = GrassState::growing;
					g.spread_attempts++;
					g.spread_indices[i] = index;
				}
			}
		}
	}
	return grass(m_grass[index].position);
}

void App::update(float dt)
{
	m_wolf.update(dt, *this);

	for (auto& g : m_grass) {
		g.update(dt, *this);
	}
	spread();
	for (auto& s : m_sheep) {
		if (!s.isAlive) continue;
		s.update(dt, *this, m_wolf.position);

		if (s.state == sheepState::reproducing) {
			s.state = sheepState::roaming;
			m_sheep.emplace_back(s.reproduce());
			s.mateposition = { 0,0 };
		}
	}
	m_sheep.erase(
        std::remove_if(m_sheep.begin(), m_sheep.end(), [](const sheep& s) {
            return !s.isAlive; 
        }), 
        m_sheep.end()
    );
	for (auto& m : m_manure) {
		m.lifetime -= dt;
		if (m.lifetime <= 0) {
			m = m_manure.back();
			m_manure.pop_back();
		}
	}
}

void App::render()
{
	for (auto& g : m_grass) {
		g.render();
	}
	for (auto& s : m_sheep) {
		s.render();
	}
	for (auto& m : m_manure) {
		m.render();
	}
	m_wolf.render();
}