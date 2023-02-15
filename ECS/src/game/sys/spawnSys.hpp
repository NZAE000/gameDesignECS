#pragma once
#include <cstdint>

//Forward declarations
struct SpawnCmp_t;
struct PhysicsCmp_t;

template<typename GameCTX_t>
struct SpawnSys_t {

	explicit SpawnSys_t() = default;

	constexpr void update(GameCTX_t&) const noexcept;

private:
	//void createNewBlade(GameCTX_t&, uint32_t x, uint32_t y) const;

};