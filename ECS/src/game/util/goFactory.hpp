#pragma once
#include <cstdint>
#include <string_view>
#include <ecs/man/entityManager.cpp>
#include <game/cmp/spawnCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <game/cmp/renderCmp.hpp>
#include <game/cmp/colliderCmp.hpp>
#include <game/util/animationManager.hpp>
#include <stdexcept>

struct AnimManager_t; // forward declaration

namespace ECS { 
	struct Entity_t;
}

struct GOFactory_t {

	GOFactory_t(AnimManager_t& am) : AnimMan{am} {}

	void setManager(ECS::EntityManager_t& em) { entityMan = &em; }

	ECS::Entity_t& createPlayer(uint32_t x, uint32_t y)    const;
	ECS::Entity_t& createBlade(uint32_t x, uint32_t y) 	   const;
	ECS::Entity_t& createPlatform(uint32_t x, uint32_t y)  const;
	ECS::Entity_t& createCamera(uint32_t x, uint32_t y, uint32_t w, uint32_t h, ECS::EntityID_t) const;

	// Programacion generica
	template<typename CALLABLE>
	ECS::Entity_t&
	createSpawner(uint32_t x, uint32_t y, CALLABLE callback) const
	{
	    auto& spwnEnt = entityMan->createEntity();

	    Appearance_t* spwAppear = AnimMan.getAppearance(CHARAC_t::SPAWNER, ACTION_t::DEFAULT);
	    if (!spwAppear)
	    	throw std::runtime_error("Spawner appearance not found");

	    [[maybe_unused]]auto& ren = entityMan->addCmp<RenderCmp_t>(spwnEnt);

	    auto& collcmp         = entityMan->addCmp<ColliderCmp_t>(spwnEnt);
	    collcmp.boxRoot       = spwAppear->boxRoot;
	    collcmp.maskCollision = ColliderCmp_t::BOUNDARY_LAYER;

	    auto& phycmp = entityMan->addCmp<PhysicsCmp_t>(spwnEnt);
	    phycmp.x     = x;
	    phycmp.y     = y;
	    phycmp.vy    = 150;

	    auto& spwncmp        = entityMan->addCmp<SpawnCmp_t>(spwnEnt);
	    spwncmp.spawnNow     = callback; // accion al momento de spawnear
	    spwncmp.tobe_spawned = 50;

	    return spwnEnt;
	}

	void createLevel1() const;
	void loadLevelFromJSON(const std::string_view path) const;
	void createBinLevelFromJSON(const std::string_view jsonpath, const std::string_view binpath) const;
	void loadLevelFromBin(const std::string_view path) const;
	
private:

	ECS::Entity_t& createEntity(uint32_t x, uint32_t y) const;

	ECS::EntityManager_t* entityMan { nullptr };
	AnimManager_t& AnimMan;
};