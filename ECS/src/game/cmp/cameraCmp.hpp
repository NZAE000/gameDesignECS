#pragma once
#include <ecs/cmp/component.hpp>
#include <cstdint>


struct CameraCmp_t : ECS::ComponentBase_t<CameraCmp_t> {

	explicit CameraCmp_t(ECS::EntityID_t eid)
	: ComponentBase_t(eid)
	{}

	uint32_t width  { 0 };
	uint32_t height { 0 };
	uint32_t xScr   { 0 };
	uint32_t yScr   { 0 };

};
