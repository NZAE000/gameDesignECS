#include <iostream>
#include <thread>
#include <game/sys/renderSys.cpp>
#include <game/sys/physicsSys.cpp>
#include <game/sys/collisionSys.cpp>
#include <game/sys/inputSys.cpp>
#include <game/sys/spawnSys.cpp>
#include <game/sys/healthSys.cpp>
#include <game/sys/cameraSys.cpp>
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

    // LEVEL 1!!
    GoFactory.createLevel1();

    // Systems
    const RenderSys_t<ECS::EntityManager_t>    Render    { WIDTH, HEIGHT };
    InputSys_t<ECS::EntityManager_t>           Input     {};
    const PhysicsSys_t<ECS::EntityManager_t>   Physic    {};
    const CollisionSys_t<ECS::EntityManager_t> Collision { WIDTH, HEIGHT };
    const SpawnSys_t<ECS::EntityManager_t>     Spawn     {};
    const HealthSys_t<ECS::EntityManager_t>    Health    {};
    const CameraSys_t<ECS::EntityManager_t>    Camera    {};

    //Render.setDebugDraw(true); // Marcado de bounding box en las entidades (solo las que tienen collider de componente)

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
        Camera.update(EntityMan);

        const auto deltaTime = clk::now() - lastTime; //const std::chrono::duration
        if (deltaTime < timePF) std::this_thread::sleep_for(timePF-deltaTime);
    }

    return 0;
    
} catch(...){
    std::cerr << "ERRO"<< '\n';
    return 1;
}
