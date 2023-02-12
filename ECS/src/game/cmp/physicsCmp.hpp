#pragma once
#include <cstdint>
#include <ecs/cmp/component.hpp>
#include <ecs/util/typeAliases.hpp>


struct PhysicsCmp_t : ECS::ComponentBase_t<PhysicsCmp_t> {

	explicit PhysicsCmp_t(ECS::EntityID_t eid) 
	: ComponentBase_t(eid) {}

	uint32_t x {0}, y {0};    // position
	int32_t vx {1}, vy {1};   // velocidad

};