#pragma once
#include <cstdint>
#include <vector>


template<typename Type>
struct BoundingBox { // plane structure (aggregate)
	
	Type xLeft  { 0 };
	Type xRight { 0 };
	Type yUp    { 0 };
	Type yDown  { 0 };
};

// Composite pattern
struct BoundingBNode {
		
	BoundingBox<uint32_t> box;
	bool isCollided { false };
	std::vector<BoundingBNode> subBoxes;
};