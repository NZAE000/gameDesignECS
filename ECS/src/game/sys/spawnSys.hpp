#pragma once
#include <cstdint>

//Forward declarations
struct SpawnCmp_t;
struct PhysicsCmp_t;

template<typename GameCTX_t>
struct SpawnSys_t {

	explicit SpawnSys_t() = default;

	void update(GameCTX_t&) const;

private:
	void createNewBlade(GameCTX_t&, uint32_t x, uint32_t y) const;

};