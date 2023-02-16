#include <game/sys/physicsSys.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <algorithm>

template<typename GameCTX_t>
constexpr bool PhysicsSys_t<GameCTX_t>::update(GameCTX_t& contx) const noexcept
{
    for (auto& phycmp : contx.template getCmps<PhysicsCmp_t>())
    {   
        // Check times vy is zero
        if (phycmp.vy == 0){
            //constexpr uint8_t zero {0};
            //phycmp.countVyZero = std::clamp(++phycmp.countVyZero, zero, phycmp.TIMES_VY_ZERO);
            if (phycmp.countVyZero < phycmp.TIMES_VY_ZERO) ++phycmp.countVyZero;

        } else phycmp.countVyZero = 0;

        // Set jump to next jump phase
        auto& jumpPhase = phycmp.jumpIndexPhase;
        if (phycmp.isInJumpPhase()) phycmp.vy = phycmp.JUMPS_PHASES[jumpPhase++];
            
        // Set speed with on gravity (only some entities with g > 0)
        phycmp.applyAccelerationY();

        // CLAMP: para que no se pase de un  limite minimo o maximo de velocidad
        phycmp.vy = std::clamp(phycmp.vy, phycmp.MIN_VY, phycmp.MAX_VY);
        //if (phycmp.vy > PhysicsCmp_t::MAX_VY) phycmp.vy = PhysicsCmp_t::MAX_VY;
        //if (phycmp.vy < PhysicsCmp_t::MIN_VY) phycmp.vy = PhysicsCmp_t::MIN_VY;

        // Set positions with on speed
        phycmp.x += phycmp.vx;
        phycmp.y += phycmp.vy;
    }
    
    return true;
}
