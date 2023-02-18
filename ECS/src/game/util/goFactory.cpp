#include <game/util/goFactory.hpp>
#include <game/cmp/renderCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <game/cmp/inputCmp.hpp>
#include <game/cmp/colliderCmp.hpp>
#include <game/cmp/spawnCmp.hpp>
#include <game/cmp/healthCmp.hpp>


ECS::Entity_t& 
GOFactory_t::createEntity(uint32_t x, uint32_t y, const std::string_view filename) const
{ 
    auto& ent = entityMan.createEntity();

    auto& rencmp = entityMan.addCmp<RenderCmp_t>(ent);
    rencmp.loadFromPng(filename);

    auto& phycmp = entityMan.addCmp<PhysicsCmp_t>(ent);
    phycmp.x     = x;
    phycmp.y     = y;

    auto& collcmp = entityMan.addCmp<ColliderCmp_t>(ent);
    collcmp.boxRoot.box = { 5, rencmp.w-5, 5, rencmp.h-5 }; // default

    [[maybe_unused]]auto& healthcmp = entityMan.addCmp<HealthCmp_t>(ent);

    return ent;
}

ECS::Entity_t& 
GOFactory_t::createPlatform(uint32_t x, uint32_t y) const
{
    auto& plataform = entityMan.createEntity();

    auto& rencmp = entityMan.addCmp<RenderCmp_t>(plataform);
    rencmp.loadFromPng("./assets/platform.png");

    auto& phycmp = entityMan.addCmp<PhysicsCmp_t>(plataform);
    phycmp.x     = x;
    phycmp.y     = y;
    phycmp.vx = phycmp.vy = 0;

    auto& collcmp = entityMan.addCmp<ColliderCmp_t>(plataform);
    collcmp.boxRoot.box   = { 0, rencmp.w, 0, rencmp.h }; // default
    collcmp.maskCollision = ColliderCmp_t::PLATFORM_LAYER;
    collcmp.property      = ColliderCmp_t::SOLID_PROP;

    return plataform;
}

ECS::Entity_t& 
GOFactory_t::createPlayer(uint32_t x, uint32_t y) const
{
    auto& principalCharac = createEntity(x, y, "./assets/deadpool2.png");
    entityMan.addCmp<InputCmp_t>(principalCharac);

    auto* collcmp = principalCharac.getCmp<ColliderCmp_t>();

    // set boundign boxes on my principal sprite player
    collcmp->boxRoot.box = { 0, 48, 0, 112 }; // 1. bounding principal
    collcmp->boxRoot.subBoxes = {
        { { 11, 47,  1,  39 }, false, // 2. subbox
            {
                { { 16, 43,  2,  8 }, false, {} }, // 3. subbox
                { { 14, 46,  8, 31 }, false, {} }, // 3.
                { { 15, 43, 31, 38 }, false, {} }  // 3.
            } 
        },
        { { 1, 47, 39, 82 }, false, // 2.
            {
                { { 4,  43, 40, 50 }, false, {} }, // 3.
                { { 2,  14, 50, 81 }, false, {} }, // 3.
                { { 21, 45, 50, 81 }, false, {} }  // 3.
            } 
        }, 
        { { 7, 45, 82, 111 }, false, // 2.
            {
                //{ {  9, 24,  83,  100 }, false, {} }, // 3.
                //{ { 31, 44,  83, 104 }, false, {} }, // 3.
                //{ {  5, 17, 100, 110 }, false, {} }, // 3.
                //{ { 33, 46, 104, 110 }, false, {} }  // 3.
            }
        }
    };

    collcmp->maskCollision = ColliderCmp_t::FULL_LAYER; // Colisiona con todo por defecto. 
    collcmp->property      = ColliderCmp_t::PLAYER_PROP;

    auto* healthcmp   = principalCharac.getCmp<HealthCmp_t>();
    healthcmp->health = 100; // set to 100 health for my player

    auto* phycmp = principalCharac.getCmp<PhysicsCmp_t>();
    phycmp->g = PhysicsCmp_t::GRAVITY; // set gravity for my player
    phycmp->vx = 10;

    return principalCharac;
}

ECS::Entity_t& 
GOFactory_t::createBlade(uint32_t x, uint32_t y) const
{
    auto& blade   = createEntity(x, y, "./assets/blade.png");

    auto* collcmp = blade.getCmp<ColliderCmp_t>();
    collcmp->maskCollision = ColliderCmp_t::BLADE_LAYER;
    collcmp->property      = ColliderCmp_t::DAMAGE_PROP;

    auto* healthcmp = blade.getCmp<HealthCmp_t>();
    healthcmp->selfDmgOnInfliction = 1;
    healthcmp->inflictedDmg        = 1;

    return blade;
}
