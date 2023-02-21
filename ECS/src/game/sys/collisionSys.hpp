#pragma once
#include <cstdint>
#include <game/cmp/colliderCmp.hpp>

// forward declaration
struct PhysicsCmp_t;


template<typename GameCTX_t>
struct CollisionSys_t {

    explicit CollisionSys_t(uint32_t wscr, uint32_t hscr) 
    : wScreen(wscr), hScreen(hscr) 
    {};

    void update(GameCTX_t&) const noexcept;

private:

    constexpr void checkBoundingScreenCollision(const BoundingBox<uint32_t>&, PhysicsCmp_t&) const noexcept;
    constexpr BoundingBox<float> transformToWorldCoordinates(const BoundingBox<uint32_t>&, float x, float y) const noexcept;
    constexpr bool isCollisionEntities(const PhysicsCmp_t&, BoundingBNode&, const PhysicsCmp_t&, BoundingBNode&) const noexcept;
    constexpr void reactBetweenEntities(GameCTX_t&, const ColliderCmp_t&, const ColliderCmp_t&) const noexcept;
    constexpr void applyDamageEntities(GameCTX_t&, const ColliderCmp_t&, const ColliderCmp_t&) const noexcept;
    constexpr void undoCollision(GameCTX_t&, const ColliderCmp_t&, const ColliderCmp_t&) const noexcept;

    const uint32_t wScreen {0}, hScreen {0};
};
