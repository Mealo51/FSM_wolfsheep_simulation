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

	editMode = false;

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
	if (IsKeyPressed(KEY_E)) editMode = !editMode; // Toggle mode

	Vector2 mPos = GetMousePosition();
	for (auto& g : m_grass) {
		if (editMode && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			// If mouse is over this tile, block it
			if (CheckCollisionPointRec(mPos, { g.position.x, g.position.y, tile_len, tile_len })) {
				g.state = GrassState::blocked;
			}
		}
		else if (editMode && IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && g.state == GrassState::blocked) {
			// If mouse is over this tile, unblock it
			if (CheckCollisionPointRec(mPos, { g.position.x, g.position.y, tile_len, tile_len })) {
				g.state = GetRandomValue(0,9) < 5 ? GrassState::growing : GrassState::dirt;
				g.grow_progress = static_cast<float>(GetRandomValue(0, 12));;
			}
		}
	}

	m_wolf.update(dt, *this);

	for (auto& g : m_grass) {
		g.update(dt, *this);
	}
	spread();

	std::vector<sheep> newsheep;
	for (auto& s : m_sheep) {
		if (!s.isAlive) continue;
		s.update(dt, *this, m_wolf.position);

		if (s.state == sheepState::reproducing) {
			s.state = sheepState::roaming;
			newsheep.emplace_back(s.reproduce());
			s.mateposition = { 0,0 };
		}
	}
	for (auto& baby : newsheep) {
		m_sheep.push_back(baby);
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
	DrawText(TextFormat("WHunger: %.1f", m_wolf.hunger), WINDOW_WIDTH - 200,
		40, 20, BLACK);
	//debug
	DrawText(TextFormat("Sheep count: %d", static_cast<int>(m_sheep.size())), WINDOW_WIDTH - 200, 20, 20, BLACK);
	Color transparentRed = { 255, 0, 0, 150 };
	Color transparentGray = { 100, 100, 100, 150 };
	Color modeColor = editMode ? transparentRed : transparentGray;
	DrawText(editMode ? "EDIT MODE : ON" : "EDIT MODE: OFF (Press E to toggle)", (WINDOW_WIDTH / 2) - 100, 10, 20, modeColor);
}