#include <game/util/goFactory.hpp>
#include <game/cmp/renderCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <game/cmp/inputCmp.hpp>
#include <game/cmp/colliderCmp.hpp>
#include <game/cmp/spawnCmp.hpp>


ECS::Entity_t& 
GOFactory::createEntity(uint32_t x, uint32_t y, const std::string_view filename) const
{ 
    auto& ent = entityMan.createEntity();

    auto& rendercmp = entityMan.addCmp<RenderCmp_t>(ent);
    rendercmp.loadFromPng(filename);

    auto& phycmp = entityMan.addCmp<PhysicsCmp_t>(ent);
    phycmp.x     = x;
    phycmp.y     = y;

    auto& collcmp = entityMan.addCmp<ColliderCmp_t>(ent);
    collcmp.boxRoot.box = { 5, rendercmp.w-5, 5, rendercmp.h-5 }; // default

    return ent;
}

ECS::Entity_t& 
GOFactory::createPlayer(uint32_t x, uint32_t y) const
{
    auto& principalCharac = createEntity(x, y, "./assets/deadpool2.png");
    entityMan.addCmp<InputCmp_t>(principalCharac);

    auto* collcmp = principalCharac.getCmp<ColliderCmp_t>();
    collcmp->boxRoot.box = { 0, 48, 0, 112 }; // 1. bounding principal

    collcmp->boxRoot.subBoxes = {
        { { 11, 47,  1,  39 }, false, // 2. subbox
            {
                { { 16, 43,  2,  8 }, false, {} }, // 3. subbox
                { { 14, 46,  8, 31 }, false, {} }, // 3.
                { { 15, 43, 31, 38 }, false, {} }  // 3.
            } 
        },
        { {  1, 47, 39, 82 }, false, // 2.
            {
                { { 4,  43, 40, 50 }, false, {} }, // 3.
                { { 2,  14, 50, 81 }, false, {} }, // 3.
                { { 21, 45, 50, 81 }, false, {} }  // 3.
            } 
        }, 
        { {  4, 47, 82, 111 }, false, // 2.
            {
                { {  9, 24,  83,  100 }, false, {} }, // 3.
                { { 31, 44,  83, 104 }, false, {} }, // 3.
                { {  5, 17, 100, 110 }, false, {} }, // 3.
                { { 33, 46, 104, 110 }, false, {} }  // 3.
            } 
        }
    };

    return principalCharac;
}

ECS::Entity_t& 
GOFactory::createBlade(uint32_t x, uint32_t y) const
{
    return createEntity(x, y, "./assets/blade.png");
}
