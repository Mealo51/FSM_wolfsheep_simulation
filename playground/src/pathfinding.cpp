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
        // 1. Coordinate Setup
        int startX = Math::tointd(floor(startPos.x / tile_len));
        int startY = Math::tointd(floor(startPos.y / tile_len));
        int endX = Math::tointd(floor(targetPos.x / tile_len));
        int endY = Math::tointd(floor(targetPos.y / tile_len));

        // Bounds Check
        if (startX < 0 || startX >= tile_x || startY < 0 || startY >= tile_y) return {};
        if (endX < 0 || endX >= tile_x || endY < 0 || endY >= tile_y) return {};

        // If target is blocked, we can't path there
        int targetIdx = (endY * (int)tile_x) + endX;
        if (world[targetIdx].state == GrassState::blocked) return {};

        // If already in the target tile, return the target position
        if (startX == endX && startY == endY) return { targetPos };

        // 2. A* Algorithm Setup
        std::vector<Node*> openList;
        std::unordered_set<int> closedList;
        std::vector<Node*> allNodes; // For memory cleanup

        Node* startNode = new Node(startX, startY);
        startNode->g = 0;
        startNode->h = GetDistance(startX, startY, endX, endY);
        openList.push_back(startNode);
        allNodes.push_back(startNode);

        while (!openList.empty()) {
            // Get node with lowest F cost
            auto currentIt = std::min_element(openList.begin(), openList.end(), [](Node* a, Node* b) {
                return a->fCost() < b->fCost();
                });

            Node* current = *currentIt;

            // Check if we reached the target
            if (current->x == endX && current->y == endY) {
                std::vector<Vector2> finalPath = retracePath(current);
                for (auto n : allNodes) delete n; // Cleanup
                return finalPath;
            }

            // Move current from open to closed
            openList.erase(currentIt);
            closedList.insert((current->y * (int)tile_x) + current->x);

            // 3. Process Neighbors (Up, Down, Left, Right)
            for (Vector2 dir : {Vector2{ 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 }}) {
                int nx = current->x + (int)dir.x;
                int ny = current->y + (int)dir.y;
                int nIndex = (ny * (int)tile_x) + nx;

                // Skip if out of bounds, blocked, or already evaluated
                if (nx < 0 || nx >= tile_x || ny < 0 || ny >= tile_y) continue;
                if (world[nIndex].state == GrassState::blocked || closedList.count(nIndex)) continue;

                float newGCost = current->g + 1.0f;

                // Check if neighbor is already in openList
                Node* neighbor = nullptr;
                for (Node* n : openList) {
                    if (n->x == nx && n->y == ny) {
                        neighbor = n;
                        break;
                    }
                }

                if (neighbor == nullptr) {
                    // Discover new node
                    Node* newNode = new Node(nx, ny);
                    newNode->g = newGCost;
                    newNode->h = GetDistance(nx, ny, endX, endY);
                    newNode->parent = current;
                    openList.push_back(newNode);
                    allNodes.push_back(newNode);
                }
                else if (newGCost < neighbor->g) {
                    // Found a better path to an existing node
                    neighbor->g = newGCost;
                    neighbor->parent = current;
                }
            }
        }

        // Cleanup memory if no path is found
        for (auto n : allNodes) delete n;
        return {};
    }
}