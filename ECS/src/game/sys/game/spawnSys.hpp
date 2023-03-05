#pragma once
#include <cstdint>
#include <ecs/sys/system.hpp>

struct SpawnSys_t : ECS::SystemBase_t<SpawnSys_t>{

	explicit SpawnSys_t() : SystemBase_t{} {}

	void update(ECS::EntityManager_t&) const;

private:
	//void createNewBlade(GameCTX_t&, uint32_t x, uint32_t y) const;

};