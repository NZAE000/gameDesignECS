#pragma once
#include <ecs/cmp/component.hpp>
#include "boundingBox.hpp"


struct ColliderCmp_t : ECS::ComponentBase_t<ColliderCmp_t> {

    // para que solo entre algunas entidades halla colision
	enum { NO_LAYER = 0X00, BLADE_LAYER = 0X01, PLATFORM_LAYER = 0X02, FULL_LAYER = 0xFF };

	explicit ColliderCmp_t(ECS::EntityID_t eid)
	: ComponentBase_t(eid) {}

	BoundingBNode boxRoot {};

	uint32_t maskCollision { FULL_LAYER }; // 256 = 1111 1111 : COLISIONAR CON TODO POR DEFAULT
};