#pragma once
//pathfinding.hpp

#include <vector>

struct grass;

namespace Pathfinding {
	struct Node {
		int x, y;          // Grid coordinates
		float g;       // Distance from start
		float h;       // Distance to end (Heuristic)
		float fCost() const { return g + h; }
		Node* parent = nullptr;

		bool operator>(const Node& other) const { return (g + h) > (other.g + other.h); }
	};

	std::vector<Vector2> findPath(Vector2 startPos, Vector2 targetPos, const std::vector<grass>& world);
}