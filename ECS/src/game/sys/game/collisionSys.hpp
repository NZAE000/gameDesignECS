#pragma once
#include <cstdint>
#include <ecs/sys/system.hpp>
#include <game/cmp/colliderCmp.hpp>

// forward declaration
struct PhysicsCmp_t;

struct CollisionSys_t : ECS::SystemBase_t<CollisionSys_t> {

    explicit CollisionSys_t(uint32_t wscr, uint32_t hscr) 
    : SystemBase_t{}, wScreen(wscr), hScreen(hscr) 
    {};

    void update(ECS::EntityManager_t&) const final override;
    void update(ECS::EntityManager_t&) final override {}

private:

    constexpr void checkBoundingScreenCollision(BoundingBox<uint32_t>const&, PhysicsCmp_t&) const noexcept;
    constexpr BoundingBox<float> transformToWorldCoordinates(const BoundingBox<uint32_t>&, float x, float y) const noexcept;
    constexpr bool isCollisionEntities(const PhysicsCmp_t&, BoundingBNode&, const PhysicsCmp_t&, BoundingBNode&) const noexcept;
    constexpr void reactBetweenEntities(ECS::EntityManager_t&, const ColliderCmp_t&, const ColliderCmp_t&) const noexcept;
    void applyDamageEntities(ECS::EntityManager_t&, const ColliderCmp_t&, const ColliderCmp_t&) const noexcept;
    void undoCollision(ECS::EntityManager_t&, const ColliderCmp_t&, const ColliderCmp_t&) const noexcept;

    const uint32_t wScreen {0}, hScreen {0};
};
