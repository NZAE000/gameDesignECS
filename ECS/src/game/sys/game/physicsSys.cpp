#include <game/sys/game/physicsSys.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <ecs/man/entityManager.cpp>
#include <algorithm>
#include <cmath>

void PhysicsSys_t::update(ECS::EntityManager_t& contx) const
{
    for (auto& phycmp : contx.template getCmps<PhysicsCmp_t>())
    {   
        // Check times vy is zero (before implemented)
        /*if (phycmp.vy == 0){
            //constexpr uint8_t zero {0};
            //phycmp.countVyZero = std::clamp(++phycmp.countVyZero, zero, phycmp.TIMES_VY_ZERO);
            if (phycmp.countVyZero < phycmp.TIMES_VY_ZERO) ++phycmp.countVyZero;

        } else phycmp.countVyZero = 0;*/

        // Verify if we are not still on platform (current solution)
        if (std::abs(phycmp.vy) > phycmp.MINVY_ONPLATFORM) phycmp.onPlatform = false;

        // Set jump to next jump phase
        auto& jumpPhase = phycmp.jumpIndexPhase;
        if (phycmp.isInJumpPhase()) phycmp.vy = phycmp.JUMPS_PHASES[jumpPhase++];
            
        // Set speed with on gravity (change only some entities with g > 0)
        phycmp.applyAccelerationY();
        
        // CLAMP: para que no se pase de un limite minimo o maximo de velocidad en y
        phycmp.vy = std::clamp(phycmp.vy, -phycmp.MAX_VY, phycmp.MAX_VY);
        //if (phycmp.vy > PhysicsCmp_t::MAX_VY) phycmp.vy = PhysicsCmp_t::MAX_VY;
        //if (phycmp.vy < PhysicsCmp_t::MIN_VY) phycmp.vy = PhysicsCmp_t::MIN_VY;

        // Set speed with acceleration x (change only some entities with ax > 0)
        phycmp.applyAccelerationX();
        phycmp.vx = std::clamp(phycmp.vx, -phycmp.MAX_VX, phycmp.MAX_VX);

        // Set positions with on speed
        phycmp.x += phycmp.vx;
        phycmp.y += phycmp.vy;
    }
}
