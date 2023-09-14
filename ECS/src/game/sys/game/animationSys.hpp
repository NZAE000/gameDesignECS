#pragma once
#include <engine/sys/system.hpp>
#include <game/util/animationManager.hpp>

namespace ECS {
	struct EntityManager_t;
}

struct AnimationSys_t : ECS::System_t<AnimationSys_t> {

	explicit AnimationSys_t(AnimManager_t& am) : System_t(), AnimMan{am} {}

	void update(ECS::EntityManager_t&) const;

private:
	constexpr void setBit(uint16_t& bin, uint32_t position) const;
	constexpr ACTION_t checkStatusAnimation() const;

	AnimManager_t& AnimMan;
};