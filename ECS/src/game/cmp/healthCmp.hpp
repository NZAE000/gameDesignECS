#pragma once
#include <engine/cmp/component.hpp>
#include <cstdint>

struct HealthCmp_t : ECS::Component_t<HealthCmp_t> {

	explicit HealthCmp_t(ECS::EntityID_t eid) 
	: Component_t(eid) {}

	uint32_t selfDmgOnInfliction { 0 };
	uint32_t inflictedDmg  		 { 0 };
	uint32_t cumulativeDmg 		 { 0 };
	uint32_t health { 1 };
};