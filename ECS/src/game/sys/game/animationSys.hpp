#pragma once
#include <ecs/sys/system.hpp>
#include <game/util/animationManager.hpp>

struct AnimationSys_t : ECS::SystemBase_t<AnimationSys_t> {

	explicit AnimationSys_t(AnimManager_t& am) : SystemBase_t(), AnimMan{am} {}

	void update(ECS::EntityManager_t&) const;

private:
	constexpr void setBit(uint16_t& bin, uint32_t position) const;
	constexpr ACTION_t checkStatusAnimation() const;

	AnimManager_t& AnimMan;
};