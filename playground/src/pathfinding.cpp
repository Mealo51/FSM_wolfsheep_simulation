// Pathfinding.cpp

#include "raylib.h"
#include <cmath>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include "pathfinding.hpp"
#include "application.hpp"
#include "grass.hpp"
#include "Constant.hpp"
#include "common.hpp"

namespace Pathfinding {
	// Helper to turn world position into grid coordinates
	Vector2 WorldToGrid(Vector2 pos) {
		return { floor(pos.x / tile_len), floor(pos.y / tile_len) };
	}

	// Standard Manhattan distance heuristic
	float GetDistance(int x1, int y1, int x2, int y2) {
		return Math::tofloat(abs(x1 - x2) + abs(y1 - y2));
	}

	std::vector<Vector2> retracePath(Node* endNode) {
		std::vector<Vector2> path;
		Node* current = endNode;

		while (current != nullptr) {
			// Convert grid coordinates back to world pixels (center of the tile)
			Vector2 worldPos = Vector2{
				static_cast<float>(current->x * tile_len + tile_len / 2.0f),
				static_cast<float>(current->y * tile_len + tile_len / 2.0f)
			};
			path.push_back(worldPos);
			current = current->parent;
		}

		// The path is currently End -> Start, so we reverse it
		std::reverse(path.begin(), path.end());
		return path;
	}

	std::vector<Vector2> findPath(Vector2 startPos, Vector2 targetPos, const std::vector<grass>& world) {
		// 1. Convert pixels to Grid Coordinates
		int startX = Math::toint(floor(startPos.x / tile_len));
		int startY = Math::toint(floor(startPos.y / tile_len));
		int endX = Math::toint(floor(targetPos.x / tile_len));
		int endY = Math::toint(floor(targetPos.y / tile_len));

		// 2. Setup lists
		std::vector<Node*> openList;
		std::unordered_set<int> closedList; // Use indices for the closed list

		Node* startNode = new Node(startX, startY);
		openList.push_back(startNode);

		while (!openList.empty()) {
			// Find node with lowest F cost
			auto currentIt = std::min_element(openList.begin(), openList.end(), [](Node* a, Node* b) {
				return a->fCost() < b->fCost();
				});
			Node* current = *currentIt;

			// Check if we reached the target
			if (current->x == endX && current->y == endY) {
				return retracePath(current); // Helper to build the Vector2 list
			}

			openList.erase(currentIt);
			closedList.insert((current->y * (int)tile_x) + current->x);

			// 3. Check Neighbors (Up, Down, Left, Right)
			for (Vector2 dir : {Vector2{ 0,1 }, { 0,-1 }, { 1,0 }, { -1,0 }}) {
				int nx = current->x + (int)dir.x;
				int ny = current->y + (int)dir.y;
				int nIndex = (ny * (int)tile_x) + nx;

				// Stay inside bounds and avoid BLOCKED tiles
				if (nx < 0 || nx >= tile_x || ny < 0 || ny >= tile_y) continue;
				if (world[nIndex].state == GrassState::blocked || closedList.count(nIndex)) continue;

				// If new path to neighbor is shorter or neighbor not in openList
				// Update neighbor and add to openList...
			}
		}
		return {}; // No path found
	}
}