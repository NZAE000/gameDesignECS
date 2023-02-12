#include <game/cmp/colliderCmp.hpp>
#include <game/cmp/healthCmp.hpp>
#include <game/sys/healthSys.hpp>
#include <iostream>

template<typename GameCTX_t>
constexpr bool HealthSys_t<GameCTX_t>::isLeafNodeCollide(const BoundingBNode& bNode) const
{
	if (bNode.subBoxes.empty()) { // leaf node, check is bounding collided
		return bNode.isCollided;
	}
	else if (bNode.isCollided) {
		for (const auto& bn : bNode.subBoxes) if (isLeafNodeCollide(bn)) return true;
	}
	return false;
}

template<typename GameCTX_t>
constexpr void HealthSys_t<GameCTX_t>::update(GameCTX_t& contx) const
{
	auto& healthCmps = contx.template getCmps<HealthCmp_t>();

	for (auto& healthcmp : healthCmps) {
		auto* collcmp = contx.template getRequiredCmp<ColliderCmp_t>(healthcmp);

		if ((healthcmp.health > 0) && isLeafNodeCollide(collcmp->boxRoot)) {

			if (--healthcmp.health == 0){
				contx.destroyEntity(healthcmp.getEntityID());
			}
			else {
				std::cout<< "Entity "<< healthcmp.getEntityID()<<" hl: "<<healthcmp.health<<"\n";
			}
		}
	}

}