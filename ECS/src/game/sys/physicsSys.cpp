#include <game/sys/physicsSys.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <algorithm>

template<typename GameCTX_t>
bool PhysicsSys_t<GameCTX_t>::update(GameCTX_t& contx) const
{
    for (auto& phycmp : contx.template getCmps<PhysicsCmp_t>())
    {   
        // Set speed with on gravity
        phycmp.vy += phycmp.g;

        // CLAMP
        phycmp.vy = std::clamp(phycmp.vy, phycmp.MIN_VY, phycmp.MAX_VY);
        //if (phycmp.vy > PhysicsCmp_t::MAX_VY) phycmp.vy = PhysicsCmp_t::MAX_VY;
        //if (phycmp.vy < PhysicsCmp_t::MIN_VY) phycmp.vy = PhysicsCmp_t::MIN_VY;

        // Set positions with on speed
        phycmp.x += phycmp.vx;
        phycmp.y += phycmp.vy;
    }
    
    return true;
}
