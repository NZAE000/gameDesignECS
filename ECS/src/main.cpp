#include <iostream>
#include <game/sys/renderSys.cpp>
#include <game/sys/physicsSys.cpp>
#include <game/sys/collisionSys.cpp>
#include <game/sys/inputSys.cpp>
#include <game/sys/spawnSys.cpp>
#include <game/sys/healthSys.cpp>
#include <ecs/man/entityManager.hpp>
#include <game/util/goFactory.hpp>

constexpr uint32_t WIDTH  { 640 };  
constexpr uint32_t HEIGHT { 360 };


int 
main(void)
try {

    ECS::EntityManager_t EntityMan;
    GOFactory GoFactory { EntityMan };

    // Entitys
    GoFactory.createPlayer(1, 1);
    //GoFactory.createBlade(20, 40);
    //GoFactory.createBlade(140, 70);

    // Entidad spawner que genera n entidades cada cierto lapso de tiempo
    // Ultimo argumento es un llamable que es la accion al momento de spawnear,
    // que en este caso es crear un nuevo blade.

    GoFactory.createSpawner(580, 1, [&](const SpawnCmp_t& spcp)
    {
        auto* phycmp = EntityMan.getRequiredCmp<PhysicsCmp_t>(spcp);
        if (!phycmp) return;

        auto& ent  = GoFactory.createBlade(phycmp->x,phycmp->y);
        phycmp     = ent.getCmp<PhysicsCmp_t>(); // los blades generados solo se desplazan en la ordenada
        phycmp->vy = 0;
    });

    // Systems
    const RenderSys_t<ECS::EntityManager_t>    Render    { WIDTH, HEIGHT };
    InputSys_t<ECS::EntityManager_t>           Input     {};
    const PhysicsSys_t<ECS::EntityManager_t>   Physic    {};
    const CollisionSys_t<ECS::EntityManager_t> Collision { WIDTH, HEIGHT };
    const SpawnSys_t<ECS::EntityManager_t>     Spawn     {};
    const HealthSys_t<ECS::EntityManager_t>    Health    {};

    Render.setDebugDraw(true); // marcado de bounding box en las entidades (solo las que tienen collider de componente)

    while (Input.update(EntityMan)){
        Render.update(EntityMan);
        Physic.update(EntityMan);
        Collision.update(EntityMan);
        Health.update(EntityMan);
        Spawn.update(EntityMan);
    }

    return 0;
    
} catch(...){
    std::cerr << "ERRO"<< '\n';
    return 1;
}
