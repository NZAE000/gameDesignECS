#pragma once
#include <cstdint>
#include <memory>
#include <game/cmp/colliderCmp.hpp>


// fordward declaration: al compilador se le dice que existe un simbolo GameContext_t como struct.
//struct GameContext_t;

struct RenderCmp_t;
struct PhysicsCmp_t;
struct CameraCmp_t;

struct CameraWithPhysics_t {

    const CameraCmp_t*  camcmp {};
    const PhysicsCmp_t* phycmp {};
};

template<typename GameCTX_t>
struct RenderSys_t {

    static constexpr uint32_t RED   = 0xFFFF0000;
    static constexpr uint32_t BLACK = 0x00000000;

    // explicit: no puede implicitamenete hacer una conversion de tipo, o algo que se pareza a RenderSys_T
    explicit RenderSys_t(uint32_t w, uint32_t h);
    ~RenderSys_t();

    constexpr void update(GameCTX_t&) const;

    // static constexpr uint32_t sprite[8*8] = {
    //     kG,kG,kG,kG,kG,kG,kG,kG
    //     ,kG,kB,kR,kR,kR,kR,kB,kG
    //     ,kG,kB,kR,kG,kG,kG,kB,kG
    //     ,kG,kB,kB,kR,kG,kG,kB,kG
    //     ,kG,kB,kB,kB,kR,kG,kB,kG
    //     ,kG,kB,kB,kB,kB,kR,kB,kG
    //     ,kG,kB,kR,kR,kR,kR,kB,kG
    //     ,kG,kG,kG,kG,kG,kG,kG,kG
    // };

    constexpr void setDebugDraw(bool state)      const noexcept { debugDraw  = state; }
    constexpr void setDebugColor(uint32_t color) const noexcept { debugColor = color; }

private:

    // Helper functions
    constexpr BoundingBox<float> transformToWorldCoordinates(const BoundingBox<uint32_t>& box, float x, float y) const noexcept;
    void drawAllEntities(const GameCTX_t&) const noexcept;
    constexpr void drawAllCameras(const GameCTX_t&) const noexcept;
    constexpr void drawSpriteClipped(const RenderCmp_t&, const PhysicsCmp_t&) const noexcept;
    constexpr void drawBoxTree(const BoundingBNode&, float x, float y, uint32_t color) const noexcept;
    constexpr void drawBox(const BoundingBox<uint32_t>&, float x, float y, uint32_t color) const noexcept;
    constexpr void drawFillBox(const BoundingBox<uint32_t>&, float x, float y, uint32_t color) const noexcept;
    constexpr void drawLineBox(uint32_t* ptr_toScr, uint32_t length, uint32_t displacement, uint32_t color) const noexcept;
    constexpr void drawAlignedLineClipped(float x, float y, uint32_t length, bool isYaxis, uint32_t color) const noexcept;
    constexpr auto transformCoordsToScreenRef(float x, float y, uint32_t width, uint32_t height) const noexcept;

    uint32_t* getPosition(uint32_t x, uint32_t y) const noexcept { return frameBuffer.get() + widthScr*y + x; }

    const uint32_t widthScr { 0 }, heightScr { 0 };
    std::unique_ptr<uint32_t[]> frameBuffer { nullptr };

    mutable bool debugDraw  { false };
    mutable uint32_t debugColor { RED };
    mutable CameraWithPhysics_t currentCam { };
};
