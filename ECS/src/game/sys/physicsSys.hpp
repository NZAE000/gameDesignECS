#pragma once


// fordward declaration: al compilador se le dice que existe un simbolo EntityManager_t como struct.
//struct GameContext_t;

template<typename GameCTX_t>
struct PhysicsSys_t {

    // explicit: no puede implicitamenete hacer una conversion de tipo, o algo quese pareza a PhysicsSys_t
    explicit PhysicsSys_t() = default;
    
    constexpr bool update(GameCTX_t&) const noexcept;

private:
    
};