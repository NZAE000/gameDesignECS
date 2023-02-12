#include <game/sys/physicsSys.hpp>
#include <game/cmp/physicsCmp.hpp>


template<typename GameCTX_t>
bool PhysicsSys_t<GameCTX_t>::update(GameCTX_t& contx) const
{
    for (auto& phy : contx.template getCmps<PhysicsCmp_t>()){
        phy.x += phy.vx;
        phy.y += phy.vy;
    }
    
    return true;
}
