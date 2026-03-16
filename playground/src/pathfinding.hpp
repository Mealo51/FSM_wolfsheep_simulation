#pragma once
//pathfinding.hpp

#include <vector>

struct grass;

namespace Pathfinding {
	struct Node {
		int x, y;          // Grid coordinates
		float gCost;       // Distance from start
		float hCost;       // Distance to end (Heuristic)
		float fCost() const { return gCost + hCost; }
		Node* parent = nullptr;

		bool operator>(const Node& other) const { return (gCost + hCost) > (other.gCost + other.hCost); }
	};

	std::vector<Vector2> findPath(Vector2 startPos, Vector2 targetPos, const std::vector<grass>& world);
}