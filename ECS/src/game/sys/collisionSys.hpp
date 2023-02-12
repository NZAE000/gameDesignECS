#pragma once
#include <cstdint>

// forward declaration
struct ColliderCmp_t;
struct PhysicsCmp_t;
struct BoundingBox;
struct BoundingBNode;

template<typename GameCTX_t>
struct CollisionSys_t {

    explicit CollisionSys_t(uint32_t wscr, uint32_t hscr) 
    : wScreen(wscr), hScreen(hscr) 
    {};

    bool update(GameCTX_t&) const;

private:

    constexpr void checkBoundingScreenCollision(const ColliderCmp_t&, PhysicsCmp_t&) const noexcept;
    constexpr BoundingBox transformToScreenCoordinates(const BoundingBox&, uint32_t x, uint32_t y) const noexcept;
    constexpr void chekCollisionEntities(const PhysicsCmp_t&, BoundingBNode&, const PhysicsCmp_t&, BoundingBNode&) const noexcept;

    const uint32_t wScreen {0}, hScreen {0};
};
