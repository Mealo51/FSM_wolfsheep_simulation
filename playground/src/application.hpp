// application.hpp

#pragma once

#include "input.hpp"
#include "Constant.hpp"
#include "entities.hpp"
#include "grass.hpp"
#include <vector>

struct App
{
	App(int width, int height);

	void update(float dt);
	void render();

	grass spread();

	float gx, gy;
	Vector2 bounds{ 0,0 };
	InputContext m_input;
	wolf m_wolf;
	std::vector<sheep> m_sheep;
	std::vector<grass> m_grass;
	std::vector<manure>m_manure;
};