#include <game/sys/collisionSys.hpp>
//#include <ecs/cmp/entity.hpp>
#include <game/cmp/colliderCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <game/cmp/healthCmp.hpp>
#include <algorithm>
#include <functional>

template<typename GameCTX_t>
constexpr BoundingBox CollisionSys_t<GameCTX_t>::
transformToScreenCoordinates(const BoundingBox& box, uint32_t x, uint32_t y) const noexcept
{
    return BoundingBox {
            x + box.xLeft
        ,   x + box.xRight
        ,   y + box.yUp
        ,   y + box.yDown 
    };
}

template<typename GameCTX_t>
constexpr void CollisionSys_t<GameCTX_t>::
checkBoundingScreenCollision(const BoundingBox& box, PhysicsCmp_t& phycmp) const noexcept
{
    // Bounding coordinates convertion to screen coordinates
    BoundingBox boxTransToSrc = transformToScreenCoordinates(box, phycmp.x, phycmp.y);
    
    uint32_t xL { boxTransToSrc.xLeft  };
    uint32_t xR { boxTransToSrc.xRight };
    uint32_t yU { boxTransToSrc.yUp    };
    uint32_t yD { boxTransToSrc.yDown  };

    // Horizontal boundig verification
    if (xL > wScreen || xR > wScreen) {
        phycmp.x -= phycmp.vx; 
        phycmp.vx *= -1; 
    }
    // Vertical boundig verification
    if (yU > hScreen || yD > hScreen) 
    {
        phycmp.y -= phycmp.vy; 

        if (phycmp.g)   phycmp.vy  =  0;  // Las entidades que tengan gravedad, entonces frenan en los limites (suelo o limite superior)
        else            phycmp.vy *= -1;  // De lo contrario las demas entidades rebotan.
    }

    /*if ( phycmp.x > (wScreen - rencmp->w) ) { // phycmp.x > wScreen || phycmp.x + rencmp->w > wScreen
        phycmp.x -= phycmp.vx; phycmp.vx *= -1; 
    }
    if ( phycmp.y > (hScreen - rencmp->h) ) { // phycmp.y > hScreen || phycmp.y + rencmp->h > hScreen)
        phycmp.y -= phycmp.vy; phycmp.vy *= -1; 
    }*/
}

template<typename GameCTX_t>
constexpr bool CollisionSys_t<GameCTX_t>::
isCollisionEntities(const PhysicsCmp_t& phycmp1, BoundingBNode& boxNode1, const PhysicsCmp_t& phycmp2, BoundingBNode& boxNode2) const noexcept
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

    auto isCollisionIntervals = [](uint32_t L1, uint32_t R1, uint32_t L2, uint32_t R2) 
    {
        return (L2 > R1 || L1 > R2)? false : true;
    };

    // Check collision in two axies (x, y)
    if ( isCollisionIntervals(box1.xLeft, box1.xRight, box2.xLeft, box2.xRight) &&
         isCollisionIntervals(box1.yUp, box1.yDown, box2.yUp, box2.yDown) ) 
    {
        /* Casos:
            1. Bn1 y Bn2 sean nodo hoja
            2. Bn1 y Bn2 tengan mas hijos (o subcajas)
            3. Bn1 sea hoja, Bn2 tenga hijos
            4. Bn2 sea hoja, Bn1 tenga hijos
        */
        boxNode1.isCollided = true;
        boxNode2.isCollided = true;

        if (!boxNode1.subBoxes.empty()){
            for (auto& b : boxNode1.subBoxes) 
                if (isCollisionEntities(phycmp1, b, phycmp2, boxNode2)) return true;
        }
        else if (!boxNode2.subBoxes.empty()){
            for (auto& b : boxNode2.subBoxes) 
                if (isCollisionEntities(phycmp1, boxNode1, phycmp2, b)) return true;
        }
        else  return true;  // cuando ambos son nodo hojas
    }
    return false;
}


template<typename GameCTX_t>
void CollisionSys_t<GameCTX_t>::update(GameCTX_t& contx) const noexcept
{
    auto& collcmps = contx.template getCmps<ColliderCmp_t>();
    uint32_t size  = collcmps.size();
    
    std::function<void(BoundingBNode&)> setFalseCollision = [&](BoundingBNode& boxNode){
        boxNode.isCollided = false;
        for (auto& b : boxNode.subBoxes) setFalseCollision(b);
    };
    // antes, setear cada componente mas sus cajas hijas como "NO COLISIONADO"
    for (auto& collcmp : collcmps) setFalseCollision(collcmp.boxRoot);

    for (uint32_t cmpi=0; cmpi<size; ++cmpi)
    {
        auto& collcmp1 = collcmps[cmpi];
        auto* phycmp1  = contx.template getRequiredCmp<PhysicsCmp_t>(collcmp1);
        if (!phycmp1) continue;

        // Fisrt check collision with screen sides ...
        checkBoundingScreenCollision(collcmp1.boxRoot.box, *phycmp1);

        // then, check collision beetween cmpcollider of an entity with cmpcollider+1 of other entity.
        for (uint32_t cmpj=cmpi+1; cmpj<size; ++cmpj) 
        {   
            auto& collcmp2 = collcmps[cmpj];
            // comprobar si tienen capa de mascara en comun para colisionar
            if ( !(collcmp1.maskCollision & collcmp2.maskCollision) ) continue;

            auto* phycmp2  = contx.template getRequiredCmp<PhysicsCmp_t>(collcmp2);
            if (!phycmp2) continue;

            if (isCollisionEntities(*phycmp1, collcmp1.boxRoot, *phycmp2, collcmp2.boxRoot))
            {
                actBetweenEntities(contx, collcmp1, collcmp2);
            }
        }
    }
}

template<typename GameCTX_t>
constexpr void CollisionSys_t<GameCTX_t>::
actBetweenEntities(GameCTX_t& contx, const ColliderCmp_t& collcmp1, const ColliderCmp_t& collcmp2) const noexcept
{
    auto* playercmp = &collcmp1;
    auto* othercmp  = &collcmp2;
    using COLLCMP   = ColliderCmp_t;

    // si el otro es el player, se intercambian direcciones de punteros
    if (othercmp->property & COLLCMP::PLAYER_PROP) std::swap(playercmp, othercmp);
    // sino, se compureba que el player realmente es player, de lo contrario no se hace nada.
    else if (!(playercmp->property & COLLCMP::PLAYER_PROP)) return;

    auto* healthPlayer { contx.template getRequiredCmp<HealthCmp_t>(*playercmp) };
    auto* healthOther  { contx.template getRequiredCmp<HealthCmp_t>(*othercmp)  };
    if (!healthPlayer || !healthOther) return;

    // Si el otro tiene propiedad de hacer daño, se genera la reaccion de daño, 
    // tanto al player como al mismo
    if (othercmp->property & COLLCMP::DAMAGE_PROP)
    {
        healthPlayer->cumulativeDmg += healthOther->inflictedDmg;
        healthOther->cumulativeDmg  += healthOther->selfDmgOnInfliction;

    }
    else if (othercmp->property & COLLCMP::SOLID_PROP){
        // react to solid collision
    }
}
