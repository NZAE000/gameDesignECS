#pragma once
#include<ecs/cmp/component.hpp>
#include<cstdint>

struct HealthCmp_t : ECS::ComponentBase_t<HealthCmp_t> {

	explicit HealthCmp_t(ECS::EntityID_t eid) 
	: ComponentBase_t(eid) {}

	uint32_t selfDmgOnInfliction { 0 };
	uint32_t inflictedDmg  		 { 0 };
	uint32_t cumulativeDmg 		 { 0 };
	uint32_t health { 1 };
};