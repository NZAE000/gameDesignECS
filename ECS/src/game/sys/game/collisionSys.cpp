#include <game/sys/game/collisionSys.hpp>
#include <ecs/man/entityManager.cpp>
#include <game/cmp/colliderCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <game/cmp/healthCmp.hpp>
#include <algorithm>
#include <functional>
#include <cmath>
#include <iostream>

constexpr BoundingBox<float> CollisionSys_t::
transformToWorldCoordinates(const BoundingBox<uint32_t>& box, float x, float y) const noexcept
{   
    // OJO!! ARM
    /*uint32_t xSpr { 
        (x >= 0)? static_cast<uint32_t>(std::round(x)) 
        : -static_cast<uint32_t>(std::abs(std::round(x)))                  
    };
    uint32_t ySpr { 
        (y >= 0)? static_cast<uint32_t>(std::round(y)) 
        : -static_cast<uint32_t>(std::abs(std::round(y))) 
    };*/

    return BoundingBox<float> {
            x + box.xLeft
        ,   x + box.xRight
        ,   y + box.yUp
        ,   y + box.yDown 
    };
}

constexpr void CollisionSys_t::
checkBoundingScreenCollision(BoundingBox<uint32_t>const& box, PhysicsCmp_t& phycmp) const noexcept
{
    // Bounding coordinates convertion to screen coordinates
    BoundingBox<float> boxTransToSrc = transformToWorldCoordinates(box, phycmp.x, phycmp.y);
    
    float xL { boxTransToSrc.xLeft  };
    float xR { boxTransToSrc.xRight };
    float yU { boxTransToSrc.yUp    };
    float yD { boxTransToSrc.yDown  };

    // Horizontal boundig verification
    if (xL >= wScreen || xR < 0) {
        phycmp.x  -= phycmp.vx; 
        phycmp.vx *= -1; 
    }
    // Vertical boundig verification
    if (yU >= hScreen || yD < 0)
    {
        phycmp.y -= phycmp.vy;
        //phycmp.jumpIndexPhase = phycmp.JUMPS_PHASES.size(); // Interrumpir salto
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

constexpr bool CollisionSys_t::
isCollisionEntities(const PhysicsCmp_t& phycmp1, BoundingBNode& boxNode1, const PhysicsCmp_t& phycmp2, BoundingBNode& boxNode2) const noexcept
{
    BoundingBox<float> box1 = transformToWorldCoordinates(boxNode1.box, phycmp1.x, phycmp1.y);
    BoundingBox<float> box2 = transformToWorldCoordinates(boxNode2.box, phycmp2.x, phycmp2.y);

    // collision on intervals (two forms):
    /*
        I1          |------------------|                                      |------------------|
                   L1                  R1                                     L1                 R1

        I2                              |---------------|   |----------------|
                                        L2             R2   L2              R2 
    */

    constexpr auto isCollisionIntervals = [](float L1, float R1, float L2, float R2) -> bool
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

        // First setON collision for these boxes (for that render system detect collision).
        boxNode1.isCollided = true;
        boxNode2.isCollided = true;

        if (!boxNode1.subBoxes.empty()){    // si box1 tuviese una o mas subcajas, chequear colision con aquellas junto a box2.
            for (auto& b : boxNode1.subBoxes) 
                if (isCollisionEntities(phycmp1, b, phycmp2, boxNode2)) return true;
        }
        else if (!boxNode2.subBoxes.empty()){ // lo mimso para box2.
            for (auto& b : boxNode2.subBoxes) 
                if (isCollisionEntities(phycmp1, boxNode1, phycmp2, b)) return true;
        }
        else  return true;  // cuando ambos son nodo hojas, entonces hubo colision hacia una parte especifica de la entidad.
    }
    
    return false;
}


void CollisionSys_t::update(ECS::EntityManager_t& contx) const
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

        // Fisrt check collision with screen sides (only entities with compatible mask collision) ...
        if (collcmp1.maskCollision & ColliderCmp_t::BOUNDARY_LAYER)
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
                reactBetweenEntities(contx, collcmp1, collcmp2);
            }
        }
    }
}

constexpr void CollisionSys_t::
reactBetweenEntities(ECS::EntityManager_t& contx, const ColliderCmp_t& collcmp1, const ColliderCmp_t& collcmp2) const noexcept
{
    auto* playercmp = &collcmp1;
    auto* othercmp  = &collcmp2;
    using COLLCMP   = ColliderCmp_t;

    // si el otro es el player, se intercambian direcciones de punteros
    if (othercmp->property & COLLCMP::PLAYER_PROP) std::swap(playercmp, othercmp);
    // sino, se compureba que el player realmente es player, de lo contrario no se hace nada.
    else if (!(playercmp->property & COLLCMP::PLAYER_PROP)) return;

    // Si el otro tiene propiedad de hacer daño, se genera la reaccion de daño, 
    // tanto al player como al mismo
    if (othercmp->property & COLLCMP::DAMAGE_PROP){
        applyDamageEntities(contx, *playercmp, *othercmp);
    }
    // Si el otro es un componente solido(plataforma p.ej), la entidad player
    // debe de posar si cae ensima del solido o impedir su recorrido ya sea por los lados o por debajo.
    else if (othercmp->property & COLLCMP::SOLID_PROP){
        undoCollision(contx, *playercmp, *othercmp);
    }
}

void CollisionSys_t::
applyDamageEntities(ECS::EntityManager_t& contx, const ColliderCmp_t& playercmp, const ColliderCmp_t& othercmp) const noexcept
{
    // Get health components
    auto* healthPlayer { contx.template getRequiredCmp<HealthCmp_t>(playercmp) };
    auto* healthOther  { contx.template getRequiredCmp<HealthCmp_t>(othercmp)  };
    if (!healthPlayer || !healthOther) return;

    healthPlayer->cumulativeDmg += healthOther->inflictedDmg;
    healthOther->cumulativeDmg  += healthOther->selfDmgOnInfliction;
}

void CollisionSys_t::
undoCollision(ECS::EntityManager_t& contx, const ColliderCmp_t& mobilecmp, const ColliderCmp_t& solidcmp) const noexcept
{   
    auto* phycmpMobile = contx.template getRequiredCmp<PhysicsCmp_t>(mobilecmp);
    auto* phycmpSolid  = contx.template getRequiredCmp<PhysicsCmp_t>(solidcmp); 
    if (!phycmpMobile || !phycmpSolid) return;   

    BoundingBox<float> boxMobile = transformToWorldCoordinates(mobilecmp.boxRoot.box, phycmpMobile->x, phycmpMobile->y);
    BoundingBox<float> boxSolid  = transformToWorldCoordinates(solidcmp.boxRoot.box, phycmpSolid->x, phycmpSolid->y);

    /* COLLISION CASES IN XAXIS (analogous for Yaxis):
    
       xL     xR    xL     xR
        |------|     |------| <- left or right colision
        |-------------------| <- center collision (small solid)   
                |---|         <- center collision (big solid)

            |-----------| <- SOLID
          xLeft      xRight 
    */

    // Al momento de colisonar, ya sea por los lados, arriba o abajo,
    // se debe desaser la colision desplazando la entidad la cantidad de pixeles que 
    // halla penetrado al solido (si es izquierda o arriba, se desplaza -pxs(neg), izquierda o abajo son +pxs(pos)).
    // Tener en cuenta que si se posa en el centro por arriba o abajo, se desplaza 0px en eje X,
    // o 0px en eje Y si esta justo al centro por los lados.

    auto deltaIntervals = [](float mobileL, float mobileR, float solidL, float solidR) 
    -> float  // Trailing return type
    {
        // if it is a collision from the left
        if ( (mobileL < solidL) && (mobileR < solidR) ){
            return solidL - mobileR; // undo must be negative, -> mobileR - solidL is positive value delta.  
        }
        // then if it is to the right
        else if (mobileR > solidR){
            return solidR - mobileL; // undo must be positive.
        }

        return 0;
    };

    // type + variable + initializer
    struct { float x, y; } undo 
    {
        deltaIntervals(boxMobile.xLeft, boxMobile.xRight, boxSolid.xLeft, boxSolid.xRight), 
        deltaIntervals(boxMobile.yUp, boxMobile.yDown, boxSolid.yUp, boxSolid.yDown)
    };

    if (undo.x == 0 || (undo.y != 0 && std::abs(undo.y) <= std::abs(undo.x))){
        //std::cout<<"undo y: "<<undo.y<<"\n";
        phycmpMobile->y  += undo.y;
        phycmpMobile->vy  = 0;
        phycmpMobile->vx *= phycmpSolid->friction;

        phycmpMobile->onPlatform = (undo.y < 0); // cuando el desacido en y es negativo, significa que estamos encima de un solido, no de lo contrario.
        phycmpMobile->jumpIndexPhase = phycmpMobile->JUMPS_PHASES.size();
    } else {
        phycmpMobile->x += undo.x;
        phycmpMobile->vx = 0;
    }

    /*int32_t undoX { 0 };
    int32_t undoY { 0 };

    // CALCULATE UNDO IN X AXIS
    // if it is a collision from the left
    if ( (boxMobile.xLeft < boxSolid.xLeft) && (boxMobile.xRight < boxSolid.xRight) ){
        undoX = boxSolid.xLeft - boxMobile.xRight; // undo must be negative, -> boxMobile.xRight - boxSolid.xLeft is positive value delta.  
    }
    // then if it is to the right
    else if (boxMobile.xRight > boxSolid.xRight){
        undoX = boxSolid.xRight - boxMobile.xLeft; // undo must be positive.
    }

    // CALCULATE UNDO IN Y AXIS
    // if it is a collision from the up
    if ( (boxMobile.yUp < boxSolid.yUp) && (boxMobile.yDown - boxSolid.yDown) ){
        undoY = boxSolid.yUp - boxMobile.yDown; // boxMobile.yDown - boxSolid.yUp 
    }
    else if ( boxMobile.yDown > boxSolid.yDown){
        undoY = boxSolid.yDown - boxMobile.yUp;
    }

    // Ahora, ver los casos en que la entidad(player) deba desplazarse desasiendo 
    // su posicion en x o en y.

    if (undoX == 0){                        // desplazar en y
    } else if (undoY == 0){                 // desplazar en x
    } else if (abs(undoY) <= abs(undoX)){   // desplazar en y
    } else if (abs(undoX) < abs(undoY)){    // desplazar en x
    }*/

}