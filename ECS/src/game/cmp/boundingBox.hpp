#pragma once
#include <cstdint>
#include <vector>
#include <game/util/box.hpp>


// Composite pattern
struct BoundingBNode {
		
	Box_t<uint32_t> box;
	bool isCollided { false };
	std::vector<BoundingBNode> subBoxes;
};