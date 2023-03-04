#pragma once
#include <cstdint>
#include <vector>
#include <game/util/box.hpp>


/*template<typename Type>
struct BoundingBox { // plane structure (aggregate)
	
	Type xLeft  { 0 };
	Type xRight { 0 };
	Type yUp    { 0 };
	Type yDown  { 0 };
};*/

// Composite pattern
struct BoundingBNode {
		
	Box_t<uint32_t> box;
	bool isCollided { false };
	std::vector<BoundingBNode> subBoxes;
};