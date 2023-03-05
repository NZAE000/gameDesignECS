#pragma once
#include <ecs/sys/system.hpp>

// forward declaration
struct BoundingBNode;

struct HealthSys_t : ECS::SystemBase_t<HealthSys_t> {

	explicit HealthSys_t() : SystemBase_t{} {}

	void update(ECS::EntityManager_t&) const;

private:
	bool isLeafNodeCollide(const BoundingBNode&) const noexcept;

};