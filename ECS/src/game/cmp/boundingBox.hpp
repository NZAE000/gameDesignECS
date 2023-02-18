#pragma once
#include <cstdint>
#include <vector>

struct BoundingBox { // plane structure (aggregate)
	
	uint32_t xLeft  { 0 };
	uint32_t xRight { 0 };
	uint32_t yUp    { 0 };
	uint32_t yDown  { 0 };
};

// Composite pattern
struct BoundingBNode {
		
	BoundingBox box;
	bool isCollided { false };
	std::vector<BoundingBNode> subBoxes;
};