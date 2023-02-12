#pragma once
#include <cstdint>
#include <string_view>
#include <ecs/man/entityManager.hpp>
#include <game/cmp/spawnCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <game/cmp/renderCmp.hpp>
#include <game/cmp/colliderCmp.hpp>

namespace ECS { 
	struct Entity_t;
	struct EntityManager_t;
}

struct GOFactory {

	GOFactory(ECS::EntityManager_t& entMan) 
	: entityMan(entMan) {}

	ECS::Entity_t& createPlayer(uint32_t x, uint32_t y) const;
	ECS::Entity_t& createBlade(uint32_t x, uint32_t y) const;
	ECS::Entity_t& createSpawner(uint32_t x, uint32_t y) const;

	// Programacion generica
	template<typename CALLABLE>
	ECS::Entity_t&
	createSpawner(uint32_t x, uint32_t y, CALLABLE callback) const
	{
	    auto& ent = entityMan.createEntity();

	    auto& spwncmp    = entityMan.addCmp<SpawnCmp_t>(ent);
	    spwncmp.spawnNow = callback; // accion al momento de spawnear
	    spwncmp.tobe_spawned = 10;

	    auto& phycmp = entityMan.addCmp<PhysicsCmp_t>(ent);
	    phycmp.x     = x;
	    phycmp.y     = y;
	    phycmp.vx    =  0;

	    auto& collcmp = entityMan.addCmp<ColliderCmp_t>(ent);
	    collcmp.maskCollision = 0; // collide with nothing.

	    return ent;
	}

private:

	ECS::Entity_t& createEntity(uint32_t x, uint32_t y, const std::string_view filename) const;

	ECS::EntityManager_t& entityMan;

};