#pragma once
#include "boundingBox.hpp"
#include <string_view>

// Most characters of game have his appearance,
// how at is his sprite with dimension (rendering) and bounding box (collision)

struct Appearance_t {

	explicit Appearance_t(std::string_view filename) { loadFromPng(filename); } // Load vector and dimensions
	explicit Appearance_t() {}

	std::vector<uint32_t> sprite;
	uint32_t w, h;
	BoundingBNode boxRoot; // bounding box and subboxes must be assigned externally

private:
	void initSpriteFromABGRData(const std::vector<unsigned char>& pixels);
	auto loadPngIntoVector(const std::string_view filename);
	void loadFromPng(const std::string_view filename);
};