#pragma once
#include <cstdint>
#include <engine/sys/system.hpp>
#include <game/cmp/colliderCmp.hpp>

// forward declaration
struct PhysicsCmp_t;

namespace ECS {
    struct EntityManager_t;
}

struct CollisionSys_t : ECS::System_t<CollisionSys_t> {

    explicit CollisionSys_t(uint32_t wscr, uint32_t hscr) 
    : System_t{}, wScreen(wscr), hScreen(hscr)
    {};

    void update(ECS::EntityManager_t&) const;

private:

    constexpr Box_t<float> transformToWorldCoordinates(const Box_t<uint32_t>&, float x, float y) const noexcept;
    constexpr void checkBoundingScreenCollision(Box_t<uint32_t>const&, PhysicsCmp_t&) const noexcept;
    constexpr bool isCollisionEntities(const PhysicsCmp_t&, BoundingBNode&, const PhysicsCmp_t&, BoundingBNode&) const noexcept;
    constexpr void reactBetweenEntities(ECS::EntityManager_t&, const ColliderCmp_t&, const ColliderCmp_t&) const noexcept;
    constexpr void applyDamageEntities(ECS::EntityManager_t&, const ColliderCmp_t&, const ColliderCmp_t&) const noexcept;
    constexpr void undoCollision(ECS::EntityManager_t&, const ColliderCmp_t&, const ColliderCmp_t&) const noexcept;

    const uint32_t wScreen {0}, hScreen {0};
};
