#pragma once
#include <engine/sys/system.hpp>

// forward declaration
struct BoundingBNode;

namespace ECS {
	struct EntityManager_t;
}

struct HealthSys_t : ECS::System_t<HealthSys_t> {

	explicit HealthSys_t() : System_t{} {}

	void update(ECS::EntityManager_t&) const;

private:
	bool isLeafNodeCollide(const BoundingBNode&) const noexcept;

};