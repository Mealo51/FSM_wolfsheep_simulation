//Constants.h
#pragma once

//window config
constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 1024;
constexpr int fps_lim = 60;

// "tile" and grass config
static float tile_len = 64.f;
static float tile_x = static_cast<float>(WINDOW_WIDTH) / tile_len;
static float tile_y = static_cast<float>(WINDOW_HEIGHT) / tile_len;
static float total_growtime = 20.f; 

//entities config
static float sheep_radius = 15.f;
static float wolf_radius = 20.f;
static float manure_radius = 5.f;