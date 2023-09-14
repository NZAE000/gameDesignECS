#pragma once
#include <engine/sys/system.hpp>

namespace ECS {
	struct EntityManager_t;
}

struct CameraSys_t : ECS::System_t<CameraSys_t>{

	explicit CameraSys_t() : System_t{} {}

	void update(ECS::EntityManager_t&) const;

};