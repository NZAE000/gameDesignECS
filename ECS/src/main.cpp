#include <iostream>
#include <thread>
#include <game/sys/renderSys.cpp>
#include <game/sys/physicsSys.cpp>
#include <game/sys/collisionSys.cpp>
#include <game/sys/inputSys.cpp>
#include <game/sys/spawnSys.cpp>
#include <game/sys/healthSys.cpp>
//#include <ecs/man/entityManager.hpp>
#include <game/util/goFactory.hpp>

// Statics by default
constexpr uint32_t WIDTH  { 640 };  
constexpr uint32_t HEIGHT { 360 };
constexpr uint32_t FPS    { 60  };
constexpr 
std::chrono::duration<double> timePF { 1000ms/FPS };


int 
main(void)
try {

    ECS::EntityManager_t EntityMan;         // Manager of entities and components
    GOFactory_t GoFactory { EntityMan };    // Game objects (entities) factory

    // --------- Entities ------------
    GoFactory.createPlayer(0, 0);
    //GoFactory.createBlade(20, 40);
    //GoFactory.createBlade(140, 70);

    // Platforms
    GoFactory.createPlatform(138, 319);
    GoFactory.createPlatform(276, 319);
    GoFactory.createPlatform(414, 319);
    GoFactory.createPlatform(215, 150); // floating plataform
    GoFactory.createPlatform(414, 240); // floating plataform

    // Entidad spawner que genera n entidades cada cierto lapso de tiempo
    // Ultimo argumento es un llamable que es la accion al momento de spawnear,
    // que en este caso es crear un nuevo blade.
    GoFactory.createSpawner(605, 1, [&](const SpawnCmp_t& spcp)
    {
        auto* phycmp = EntityMan.getRequiredCmp<PhysicsCmp_t>(spcp);
        if (!phycmp) return;

        auto& ent  = GoFactory.createBlade(phycmp->x,phycmp->y);
        phycmp     = ent.getCmp<PhysicsCmp_t>(); // los blades generados solo se desplazan en la ordenada
        phycmp->vx = -2;
        phycmp->vy =  0;
    });

    // Systems
    const RenderSys_t<ECS::EntityManager_t>    Render    { WIDTH, HEIGHT };
    InputSys_t<ECS::EntityManager_t>           Input     {};
    const PhysicsSys_t<ECS::EntityManager_t>   Physic    {};
    const CollisionSys_t<ECS::EntityManager_t> Collision { WIDTH, HEIGHT };
    const SpawnSys_t<ECS::EntityManager_t>     Spawn     {};
    const HealthSys_t<ECS::EntityManager_t>    Health    {};

    Render.setDebugDraw(true); // Marcado de bounding box en las entidades (solo las que tienen collider de componente)

    using clk = std::chrono::steady_clock;
    clk::time_point lastTime;

    while (Input.update(EntityMan))
    {   
        lastTime = clk::now();

        Render.update(EntityMan);
        Physic.update(EntityMan);
        Collision.update(EntityMan);
        Health.update(EntityMan);
        Spawn.update(EntityMan);

        const auto deltaTime = clk::now() - lastTime; //const std::chrono::duration
        if (deltaTime < timePF) std::this_thread::sleep_for(timePF-deltaTime);
    }

    return 0;
    
} catch(...){
    std::cerr << "ERRO"<< '\n';
    return 1;
}
