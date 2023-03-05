#pragma once
#include <ecs/sys/system.hpp>

struct CameraSys_t : ECS::SystemBase_t<CameraSys_t>{

	explicit CameraSys_t() : SystemBase_t{} {}

	void update(ECS::EntityManager_t&) const;

};