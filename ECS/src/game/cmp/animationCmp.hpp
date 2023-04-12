#pragma once
#include <ecs/cmp/component.hpp>
#include <game/util/typeAnimations.hpp>

struct AnimationCmp_t : ECS::ComponentBase_t<AnimationCmp_t> {

	explicit AnimationCmp_t(ECS::EntityID_t eid)
	: ComponentBase_t(eid) {}

	CHARAC_t character;
	ACTION_t currentAction   { ACTION_t::DEFAULT };
	FRAME_SEQUENCE  currentFrameSec  { 0 };
	REP_PER_FRAME   currentRepFrame  { 0 };
};