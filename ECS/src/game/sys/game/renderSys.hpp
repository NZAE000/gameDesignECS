#pragma once
#include <cstdint>
#include <ecs/sys/system.hpp>
#include <game/cmp/colliderCmp.hpp>
#include <game/util/gameBuffer.hpp>

// fordward declaration: al compilador se le dice que existe un simbolo GameContext_t como struct.
//struct GameContext_t;

struct RenderCmp_t;
struct PhysicsCmp_t;
struct CameraCmp_t;

struct CameraWithPhysics_t {

    const CameraCmp_t*  camcmp {};
    const PhysicsCmp_t* phycmp {};
    std::size_t eid;
};

struct RenderSys_t : ECS::SystemBase_t<RenderSys_t> {

    static constexpr uint32_t RED           { 0xFFFF0000 };
    static constexpr uint32_t BACKGRD_COLOR { 0x0017202A };//  17202a

    // explicit: no puede implicitamenete hacer una conversion de tipo, o algo que se pareza a RenderSys_T
    explicit RenderSys_t(FrameBuffer_t& buff) 
    : SystemBase_t{}, fBuff{buff} {}

    ~RenderSys_t(){ }  

    void update(ECS::EntityManager_t&) const;

    constexpr void setDebugDraw(bool state)       const noexcept { debugDraw   = state; }
    constexpr void setDebugColor(uint32_t color)  const noexcept { debugColor  = color; }

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

private:

    // Helper functions members
    void renderCamFrame() const noexcept;
    void renderAllCameras(const ECS::EntityManager_t&) const;
    void renderAllEntities(const ECS::EntityManager_t&) const;
    void renderSpriteClipped(const RenderCmp_t&, const PhysicsCmp_t&) const noexcept;
    void renderBoxTree(const BoundingBNode&, float x, float y, uint32_t color) const noexcept;
    void renderBoxClipped(const Box_t<uint32_t>&, float x, float y, uint32_t color) const noexcept;
    void renderFillBoxClipped(const Box_t<uint32_t>&, float x, float y, uint32_t color) const noexcept;
    void renderAlignedLineClipped(float x, float y, uint32_t length, bool isYaxis, uint32_t color) const noexcept;
    
    constexpr Box_t<float> transformToWorldCoordinates(const Box_t<uint32_t>& box, float x, float y) const noexcept;
    auto transformWorldCoordsToScreenRef(float x, float y, uint32_t width, uint32_t height) const noexcept;


    FrameBuffer_t& fBuff;
    mutable bool debugDraw  { false };
    mutable uint32_t debugColor { RED };
    mutable CameraWithPhysics_t currentCam {};
};
