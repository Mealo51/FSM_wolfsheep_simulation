//Constants.h
#pragma once

//window config
constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 1024;
constexpr int fps_lim = 60;

// "tile" config
static float tile_len = 20.f;
static int tile_x = 1024 / tile_len;
static int tile_y = 1024 / tile_len;
static float tick_rate = 1.f; 
//1200 tick for 20 sec 600 tick for 10 sec growing the grass (growth_rate will be modified in grass.cpp)

//entities config
static float sheep_radius = 15.f;
static float wolf_radius = 20.f;