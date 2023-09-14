#pragma once
#include <engine/sys/system.hpp>

namespace ECS {
	struct EntityManager_t;
}

struct SpawnSys_t : ECS::System_t<SpawnSys_t>{

	explicit SpawnSys_t() : System_t{} {}

	void update(ECS::EntityManager_t&) const;

private:
	//void createNewBlade(GameCTX_t&, uint32_t x, uint32_t y) const;

};