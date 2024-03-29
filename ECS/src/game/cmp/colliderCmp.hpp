#pragma once
#include <engine/cmp/component.hpp>
#include <game/util/boundingBox.hpp>


struct ColliderCmp_t : ECS::Component_t<ColliderCmp_t> {

    // Para que solo entre algunas entidades halla colision
	enum { NO_LAYER = 0X00, BLADE_LAYER = 0X01, PLATFORM_LAYER = 0X02, BOUNDARY_LAYER = 0X04, FULL_LAYER = 0xFF };

	// Para diferenciar entre tipos de colisiones: si se colisona con el que tiene 
	// la propiedad DAMAGE, se sabe que hace daño a la entidad.
	enum { NO_PROP = 0X00, PLAYER_PROP = 0X01, SOLID_PROP = 0x02, DAMAGE_PROP = 0X04 }; 

	explicit ColliderCmp_t(ECS::EntityID_t eid)
	: Component_t(eid) {}

	BoundingBNode boxRoot;
	uint8_t maskCollision { NO_LAYER }; // 0 = 0000 0000 : COLISIONA CON NADA POR DEFAULT
	uint8_t property { NO_PROP };
};
