#include <game/sys/collisionSys.hpp>
//#include <ecs/cmp/entity.hpp>
#include <game/cmp/colliderCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <functional>

template<typename GameCTX_t>
constexpr BoundingBox CollisionSys_t<GameCTX_t>::
transformToScreenCoordinates(const BoundingBox& box, uint32_t x, uint32_t y) const noexcept
{
    return BoundingBox {
            box.xLeft  + x
        ,   box.xRight + x
        ,   box.yUp    + y
        ,   box.yDown  + y 
    };
}

template<typename GameCTX_t>
constexpr void CollisionSys_t<GameCTX_t>::
checkBoundingScreenCollision(const ColliderCmp_t& collcmp, PhysicsCmp_t& phycmp) const noexcept
{
    // Coordinates convertion to screen coordinates
    BoundingBox box = transformToScreenCoordinates(collcmp.boxRoot.box, phycmp.x, phycmp.y);
    
    uint32_t xL { box.xLeft  };
    uint32_t xR { box.xRight };
    uint32_t yU { box.yUp    };
    uint32_t yD { box.yDown  };

    // Horizontal boundig verification
    if (xL > wScreen || xR > wScreen) {
        phycmp.x -= phycmp.vx; phycmp.vx *= -1; 
    }
    // Vertical boundig verification
    if (yU > hScreen || yD > hScreen) {
        phycmp.y -= phycmp.vy; phycmp.vy *= -1; 
    }

    /*if ( phycmp.x > (wScreen - rencmp->w) ) { // phycmp.x > wScreen || phycmp.x + rencmp->w > wScreen
        phycmp.x -= phycmp.vx; phycmp.vx *= -1; 
    }
    if ( phycmp.y > (hScreen - rencmp->h) ) { // phycmp.y > hScreen || phycmp.y + rencmp->h > hScreen)
        phycmp.y -= phycmp.vy; phycmp.vy *= -1; 
    }*/
}

template<typename GameCTX_t>
constexpr void CollisionSys_t<GameCTX_t>::
chekCollisionEntities(const PhysicsCmp_t& phycmp1, BoundingBNode& boxNode1, const PhysicsCmp_t& phycmp2, BoundingBNode& boxNode2) const noexcept
{
    BoundingBox box1 = transformToScreenCoordinates(boxNode1.box, phycmp1.x, phycmp1.y);
    BoundingBox box2 = transformToScreenCoordinates(boxNode2.box, phycmp2.x, phycmp2.y);

    // collision on intervals (two forms):
    /*
        I1          |------------------|                                      |------------------|
                   L1                  R1                                     L1                 R1

        I2                              |---------------|   |----------------|
                                        L2             R2   L2              R2 
    */

    auto checkCollisionIntervals = [](uint32_t L1, uint32_t R1, uint32_t L2, uint32_t R2) 
    {
        return (L2 > R1 || L1 > R2)? false : true;
    };

    // check collision in two axies (x, y)
    if ( checkCollisionIntervals(box1.xLeft, box1.xRight, box2.xLeft, box2.xRight) &&
         checkCollisionIntervals(box1.yUp, box1.yDown, box2.yUp, box2.yDown) ) 
    {
        /* Casos:

            1. Bn1 y Bn2 sean nodo hoja
            2. Bn1 y Bn2 tengan mas hijos (o subcajas)
            3. Bn1 sea hoja, Bn2 tenga hijos
            4. Bn2 sea hoja, Bn1 tenga hijos
        */

        if (!boxNode1.subBoxes.empty()){
            for (auto& b : boxNode1.subBoxes) chekCollisionEntities(phycmp1, b, phycmp2, boxNode2);
        }
        else if (!boxNode2.subBoxes.empty()){
            for (auto& b : boxNode2.subBoxes) chekCollisionEntities(phycmp1, boxNode1, phycmp2, b);
        }
        //else { // ambos son nodo hojas
            boxNode1.isCollided = true;
            boxNode2.isCollided = true;
        //}
    }
}


template<typename GameCTX_t>
bool CollisionSys_t<GameCTX_t>::update(GameCTX_t& contx) const
{
    auto& collcmps = contx.template getCmps<ColliderCmp_t>();
    uint32_t size  = collcmps.size();

    // antes, setear cada componente mas sus cajas hijas como "NO COLISIONADO"
    std::function<void(BoundingBNode&)> setFalseCollision = [&](BoundingBNode& boxNode){
        boxNode.isCollided = false;
        for (auto& b : boxNode.subBoxes) setFalseCollision(b);
    };

    for (auto& collcmp : collcmps) setFalseCollision(collcmp.boxRoot);

    for (uint32_t cmpi=0; cmpi<size; ++cmpi)
    {
        auto& collcmp1 = collcmps[cmpi];
        auto* phycmp1  = contx.template getRequiredCmp<PhysicsCmp_t>(collcmp1);
        if (!phycmp1) continue;

        // fisrt check collision with screen sides
        checkBoundingScreenCollision(collcmp1, *phycmp1);

        // then, check collision beetween cmpcollider of an entity with cmpcollider+1 of other entity.
        for (uint32_t cmpj=cmpi+1; cmpj<size; ++cmpj) 
        {   
            auto& collcmp2 = collcmps[cmpj];
            // comprobar si tienen capa de mascara en comun
            if ( !(collcmp1.maskCollision & collcmp2.maskCollision) ) continue;

            auto* phycmp2  = contx.template getRequiredCmp<PhysicsCmp_t>(collcmp2);
            if (!phycmp2) continue;

            chekCollisionEntities(*phycmp1, collcmp1.boxRoot, *phycmp2, collcmp2.boxRoot); // check here!
        }
    }

    return true;
}
