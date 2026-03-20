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
			// TARGET THE CENTER: (Index * Size) + (Half Size)
			Vector2 worldPos = Vector2{
				(current->x * tile_len) + (tile_len / 2.0f),
				(current->y * tile_len) + (tile_len / 2.0f)
			};
			path.push_back(worldPos);
			current = current->parent;
		}

		std::reverse(path.begin(), path.end());
		return path;
	}

	std::vector<Vector2> findPath(Vector2 startPos, Vector2 targetPos, const std::vector<grass>& world) {
		// 1. Convert pixels to Grid Coordinates
		int startX = Math::toint(floor(startPos.x / tile_len));
		int startY = Math::toint(floor(startPos.y / tile_len));
		int endX = Math::toint(floor(targetPos.x / tile_len));
		int endY = Math::toint(floor(targetPos.y / tile_len));

		// 1. Safety: If start and end are the same tile, just go straight to target
		if (startX == endX && startY == endY) {
			return { targetPos };
		}

		// 2. Bounds Check
		if (endX < 0 || endX >= tile_x || endY < 0 || endY >= tile_y) return {};

		// 3. If target is a WALL, find path to the nearest open neighbor instead of failing
		int targetIdx = (endY * (int)tile_x) + endX;
		if (world[targetIdx].state == GrassState::blocked) return {};

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

				if (nx < 0 || nx >= tile_x || ny < 0 || ny >= tile_y) continue;
				if (world[nIndex].state == GrassState::blocked || closedList.count(nIndex)) continue;

				// Calculate the cost to move to this neighbor
				float newGCost = current->gCost + 1.0f; // Each tile costs 1

				// Check if neighbor is already in openList
				Node* neighbor = nullptr;
				for (Node* n : openList) {
					if (n->x == nx && n->y == ny) {
						neighbor = n;
						break;
					}
				}

				if (neighbor == nullptr) {
					// New node: create it and add to openList
					Node* newNode = new Node(nx, ny);
					newNode->gCost = newGCost;
					newNode->hCost = GetDistance(nx, ny, endX, endY);
					newNode->parent = current;
					openList.push_back(newNode);
				}
				else if (newGCost < neighbor->gCost) {
					// Better path found to an existing node: update it
					neighbor->gCost = newGCost;
					neighbor->parent = current;
				}
			}
		}
		return {}; // No path found
	}
}