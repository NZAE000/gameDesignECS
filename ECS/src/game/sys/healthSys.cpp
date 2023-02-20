#include <game/cmp/colliderCmp.hpp>
#include <game/cmp/healthCmp.hpp>
#include <game/sys/healthSys.hpp>
//#include <iostream>

template<typename GameCTX_t>
constexpr bool HealthSys_t<GameCTX_t>::isLeafNodeCollide(const BoundingBNode& bNode) const noexcept
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
constexpr void HealthSys_t<GameCTX_t>::update(GameCTX_t& contx) const noexcept
{
	auto& healthCmps = contx.template getCmps<HealthCmp_t>();

	for (auto& healthcmp : healthCmps) 
	{
		//auto id = healthcmp.getEntityID();

		if (healthcmp.cumulativeDmg) 
		{
			if (healthcmp.cumulativeDmg >= healthcmp.health) {
				contx.destroyEntity(healthcmp.getEntityID());
				//std::cout<<"Entity "<<id<<" destroyed\n";
			}
			else {
				healthcmp.health -= healthcmp.cumulativeDmg;
				healthcmp.cumulativeDmg = 0;
				//std::cout<< "Entity "<<id<<" hl: "<<healthcmp.health<<"\n";
			}
		} 
		/*auto* collcmp = contx.template getRequiredCmp<ColliderCmp_t>(healthcmp);

		if ((healthcmp.health > 0) && isLeafNodeCollide(collcmp->boxRoot)) 
		{
			auto id = healthcmp.getEntityID();
			if (--healthcmp.health == 0){
				contx.destroyEntity(healthcmp.getEntityID());
			 	std::cout<<"Entity "<<id<<" destroyed\n";
			}
			else {
				std::cout<< "Entity "<<id<<" hl: "<<healthcmp.health<<"\n";
			}
		}*/
	}

}