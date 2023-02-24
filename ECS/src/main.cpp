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
#include <game/util/gameTimer.hpp>

// Statics by default
constexpr uint32_t WIDTH  { 640 };  
constexpr uint32_t HEIGHT { 360 };
constexpr uint32_t FPS    { 60  };
constexpr uint64_t timePF { 1000000000UL/FPS };


int 
main(void)
try {

    ECS::EntityManager_t EntityMan;         // Manager of entities and components
    GOFactory_t GoFactory { EntityMan };    // Game objects (entities) factory

    auto measureTimeToProcc = [](auto&& proccess) -> double { // Trailing return type
        GameTimer_t timer {};
        proccess();
        return static_cast<double>(timer.timePassed())/1000; // ms
    };

    // LEVEL 1!!
    //std::cout << measureTimeToProcc([&](){ GoFactory.loadLevelFromJSON("./assets/levels/level1.json"); })<<"\n";
    //GoFactory.createBinLevelFromJSON("./assets/levels/level1.json", "./assets/levels/Level1.bin");
    std::cout << measureTimeToProcc([&](){ GoFactory.loadLevelFromBin("./assets/levels/Level1.bin"); }) <<"\n";

    // Systems
    const RenderSys_t<ECS::EntityManager_t>    Render    { WIDTH, HEIGHT };
    InputSys_t<ECS::EntityManager_t>           Input     {};
    const PhysicsSys_t<ECS::EntityManager_t>   Physic    {};
    const CollisionSys_t<ECS::EntityManager_t> Collision { WIDTH, HEIGHT };
    const SpawnSys_t<ECS::EntityManager_t>     Spawn     {};
    const HealthSys_t<ECS::EntityManager_t>    Health    {};
    const CameraSys_t<ECS::EntityManager_t>    Camera    {};

    //Render.setDebugDraw(true); // Marcado de bounding box en las entidades (solo las que tienen collider de componente)

    GameTimer_t timer {};

    while (Input.update(EntityMan))
    {   
        Render.update(EntityMan);
        Physic.update(EntityMan);
        Collision.update(EntityMan);
        Health.update(EntityMan);
        Spawn.update(EntityMan);
        Camera.update(EntityMan);

        timer.waitForUntil_ns(timePF);
        timer.start();
    }

    return 0;

} 
catch(const std::exception& e){
    std::cerr <<"[[Exception]]: "<< e.what() << '\n';
    return 1;
}   
catch(...){
    std::cerr << "ERRO"<< '\n';
    return 1;
}
