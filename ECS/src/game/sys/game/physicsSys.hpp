#pragma once
#include <engine/sys/system.hpp>

// fordward declaration: al compilador se le dice que existe un simbolo EntityManager_t como struct.

namespace ECS {
    struct EntityManager_t;
}

struct PhysicsSys_t : ECS::System_t<PhysicsSys_t>{

    // explicit: no puede implicitamenete hacer una conversion de tipo, o algo quese pareza a PhysicsSys_t
    explicit PhysicsSys_t() : System_t{} {}
    
    // ALL UNITS OF MOVEMENTS ARE RELATIVE TO TIME
    void update(ECS::EntityManager_t&, double dt) const noexcept;

private:
    
};