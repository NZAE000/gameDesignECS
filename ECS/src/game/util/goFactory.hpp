#pragma once
#include <cstdint>
#include <string_view>
#include <ecs/man/entityManager.cpp>
#include <game/cmp/spawnCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <game/cmp/renderCmp.hpp>
#include <game/cmp/colliderCmp.hpp>

namespace ECS { 
	struct Entity_t;
}

struct GOFactory {

	GOFactory(ECS::EntityManager_t& entMan) 
	: entityMan(entMan) {}

	ECS::Entity_t& createPlayer(uint32_t x, uint32_t y) const;
	ECS::Entity_t& createBlade(uint32_t x, uint32_t y) const;

	// Programacion generica
	template<typename CALLABLE>
	ECS::Entity_t&
	createSpawner(uint32_t x, uint32_t y, CALLABLE callback) const
	{
	    auto& spwnEnt = entityMan.createEntity();

	    [[maybe_unused]]auto& ren = entityMan.addCmp<RenderCmp_t>(spwnEnt);

	    auto& spwncmp        = entityMan.addCmp<SpawnCmp_t>(spwnEnt);
	    spwncmp.spawnNow     = callback; // accion al momento de spawnear
	    spwncmp.tobe_spawned = 50;

	    auto& phycmp = entityMan.addCmp<PhysicsCmp_t>(spwnEnt);
	    phycmp.x     = x;
	    phycmp.y     = y;
	    phycmp.vx    =  0;

	    auto& collcmp         = entityMan.addCmp<ColliderCmp_t>(spwnEnt);
	    collcmp.boxRoot.box   = { 0, 40, 0, 40 };
	    collcmp.maskCollision = 0; // collide with nothing.

	    return spwnEnt;
	}

private:

	ECS::Entity_t& createEntity(uint32_t x, uint32_t y, const std::string_view filename) const;

	ECS::EntityManager_t& entityMan;

};