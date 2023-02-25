#pragma once
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
#include <game/man/state.hpp>

// Statics by default
constexpr uint32_t WIDTH  { 640 };  
constexpr uint32_t HEIGHT { 360 };
constexpr uint32_t FPS    { 60  };
constexpr uint64_t timePF { 1000000000UL/FPS };


// OJO: int&& a = 5; <-- Referencia a un rvalue o valor temporal. En cambio, en el procesos de deducción de un parametro 'auto' en un lamda
//, (que se trata como plantilla) el doble ampresand le dice al compilador que tenga la libertad de decucir si se esta pasando un rvalue, como un lvalue.
auto measureTimeToProcc = [](auto&& proccess) -> double { // Trailing return type
    GameTimer_t timer {};
    proccess();
    return static_cast<double>(timer.timePassed())/1000; // ms
};

struct GameMan_t : State_t {

	explicit GameMan_t() {

	    // LEVEL 1!!
	    //std::cout << measureTimeToProcc([&](){ GoFactory.loadLevelFromJSON("./assets/levels/level1.json"); })<<"\n";
	    //GoFactory.createBinLevelFromJSON("./assets/levels/level1.json", "./assets/levels/Level1.bin");
	    std::cout << measureTimeToProcc([&](){ GoFactory.loadLevelFromBin("./assets/levels/Level1.bin"); }) <<"\n";

	    //Render.setDebugDraw(true); // Marcado de bounding box en las entidades (solo las que tienen collider de componente)
	}

	void update() final override
	{
		timer.start();

        Render.update(EntityMan);
        Physic.update(EntityMan);
        Input.update(EntityMan);
        Collision.update(EntityMan);
        Health.update(EntityMan);
        Spawn.update(EntityMan);
        Camera.update(EntityMan);

        timer.waitForUntil_ns(timePF);

        if (Input.isKeyPress(XK_Escape)) activeState = false;
	}

	bool isActiveState() final override { return activeState; }

private:

	ECS::EntityManager_t EntityMan;         // Manager of entities and components
    GOFactory_t GoFactory { EntityMan };    // Game objects (entities) factory

    // Systems
    const RenderSys_t<ECS::EntityManager_t>    Render    { WIDTH, HEIGHT };
    InputSys_t<ECS::EntityManager_t>           Input     {};
    const PhysicsSys_t<ECS::EntityManager_t>   Physic    {};
    const CollisionSys_t<ECS::EntityManager_t> Collision { WIDTH, HEIGHT };
    const SpawnSys_t<ECS::EntityManager_t>     Spawn     {};
    const HealthSys_t<ECS::EntityManager_t>    Health    {};
    const CameraSys_t<ECS::EntityManager_t>    Camera    {};

    GameTimer_t timer {};
    bool activeState { true };

};