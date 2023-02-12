#pragma once
#include <ecs/cmp/component.hpp>
#include "boundingBox.hpp"


struct ColliderCmp_t : ECS::ComponentBase_t<ColliderCmp_t> {

	explicit ColliderCmp_t(ECS::EntityID_t eid)
	: ComponentBase_t(eid) {}

	BoundingBNode boxRoot {};

	// para que solo entre algunas entidades halla colision
	uint32_t maskCollision { 0xFF }; // 256 = 1111 1111
};