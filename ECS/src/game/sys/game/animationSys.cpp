#include <game/sys/game/animationSys.hpp>
#include <engine/man/entityManager.hpp>
#include <game/cmp/animationCmp.hpp>
#include <game/cmp/renderCmp.hpp>
#include <game/cmp/colliderCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <game/util/appearance.hpp>
#include <cmath>

constexpr void AnimationSys_t::setBit(uint16_t& bin, uint32_t position) const
{
	bin |= static_cast<uint16_t>(std::pow(2, position));
}

void AnimationSys_t::update(ECS::EntityManager_t& contx) const
{
	auto& animcmps = contx.template getCmps<AnimationCmp_t>();
	for (auto& animcmp : animcmps)
	{
		AppearFrames_t* appearFrame = AnimMan.getAppearance(animcmp.character, animcmp.currentAction, animcmp.currentFrameSec);
		if (!appearFrame) continue;

		auto* phycmp = contx.template getRequiredCmp<PhysicsCmp_t>(animcmp);
		if (!phycmp) continue;

		if (phycmp->vx > 0)      AnimMan.onNormalAnimations();
		else if (phycmp->vx < 0) AnimMan.onFlippingAnimations();

		if (++animcmp.currentRepFrame == appearFrame->rep)
		{
			auto* rencmp = contx.template getRequiredCmp<RenderCmp_t>(animcmp);
			if (!rencmp) continue;

			auto* collcmp = contx.template getRequiredCmp<ColliderCmp_t>(*rencmp);
			if (!collcmp) continue;

			rencmp->sprite   = appearFrame->appear.sprite.data();
	    	rencmp->w        = appearFrame->appear.w;
	    	rencmp->h        = appearFrame->appear.h;
	   		collcmp->boxRoot = appearFrame->appear.boxRoot;

	   		++animcmp.currentFrameSec;
	   		animcmp.currentRepFrame = 0;
   		}
	}
}