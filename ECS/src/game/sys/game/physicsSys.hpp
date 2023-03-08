#pragma once
#include <ecs/sys/system.hpp>

// fordward declaration: al compilador se le dice que existe un simbolo EntityManager_t como struct.
//struct GameContext_t;

struct PhysicsSys_t : ECS::SystemBase_t<PhysicsSys_t>{

    // explicit: no puede implicitamenete hacer una conversion de tipo, o algo quese pareza a PhysicsSys_t
    explicit PhysicsSys_t() : SystemBase_t{} {}
    
    // ALL UNITS OF MOVEMENTS ARE RELATIVE TO TIME
    void update(ECS::EntityManager_t&, double dt) const noexcept;

private:
    
};