#pragma once

// forward declaration
struct BoundingBNode;


template<typename GameCTX_t>
struct HealthSys_t {

	constexpr void update(GameCTX_t&) const;

private:
	constexpr bool isLeafNodeCollide(const BoundingBNode&) const;

};