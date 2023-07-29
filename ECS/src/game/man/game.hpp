#pragma once
#include <game/sys/game/renderSys.hpp>
#include <game/sys/game/physicsSys.hpp>
#include <game/sys/game/collisionSys.hpp>
#include <game/sys/game/inputSys.hpp>
#include <game/sys/game/spawnSys.hpp>
#include <game/sys/game/healthSys.hpp>
#include <game/sys/game/cameraSys.hpp>
#include <game/sys/game/animationSys.hpp>
#include <ecs/man/entityManager.hpp>
#include <game/util/gameTimer.hpp>
#include <game/man/stateManager.hpp>
#include <game/man/state.hpp>
extern "C" {
    #include <tinyPTC.ua/src/tinyptc.h>
}


// Statics by default
constexpr uint32_t FPS    { 60  };
constexpr uint64_t timePF { 1000000000UL/FPS };

struct Pause_t : State_t {

    explicit Pause_t() = default;

    void update() final override
    {
        char c {};
        std::cout<<"PAUSE..\n\n";
        std::cin.get(c);

        std::cin.ignore(1000000000, '\n');
        std::cin.clear();

        activeState = false;
    }

    bool isActiveState() final override { return activeState; }

private:
    bool activeState { true };

};


/*// OJO: int&& a = 5; <-- Referencia a un rvalue o valor temporal. En cambio, en el procesos de deducción de un parametro 'auto' en un lamda
//, (que se trata como plantilla) el doble ampresand le dice al compilador que tenga la libertad de decucir si se esta pasando un rvalue, como un lvalue.
auto measureTimeToProcc = [](auto&& proccess) -> double { // Trailing return type
    GameTimer_t timer {};
    proccess();
    return static_cast<double>(timer.timePassed())/1000000; // ms
};*/

struct GameMan_t : State_t {

	explicit GameMan_t(StateManager_t& sm)
    : stateMan{sm} 
    // GET ALL SYSTEMS REF TO USE
    ,   RenSys { stateMan.getSys<const RenderSys_t>()    }
    ,   PhySys { stateMan.getSys<const PhysicsSys_t>()   }
    ,   InpSys { stateMan.getSys<const InputSys_t>()     }
    ,   ColSys { stateMan.getSys<const CollisionSys_t>() }
    ,   HthSys { stateMan.getSys<const HealthSys_t>()    }
    ,   SpwSys { stateMan.getSys<const SpawnSys_t>()     }
    ,   CamSys { stateMan.getSys<const CameraSys_t>()    }
    ,   AnmSys { stateMan.getSys<const AnimationSys_t>() }
    {
	    // LEVEL 1!!
	    //std::cout << measureTimeToProcc([&](){ stateMan.getFactory().loadLevelFromJSON("./assets/levels/level1.json"); })<<"\n";
	    //stateMan.getFactory().createBinLevelFromJSON("./assets/levels/level1.json", "./assets/levels/Level1.bin");
	   
        stateMan.setManager(entityMan);
        InpSys.setOn();
        RenSys.setDebugDraw(false);

        std::cout << timer.measureTimeToProcc([&](){ stateMan.getFactory().loadLevelFromBin("./assets/levels/Level1.bin"); }) <<"\n";
    }

    //~GameMan_t() { ptc_close(); }

	void update() final override
	{
		timer.start();

        RenSys.update(entityMan);
        PhySys.update(entityMan, 1.0/FPS);
        AnmSys.update(entityMan);
        InpSys.update(entityMan);
        ColSys.update(entityMan);
        HthSys.update(entityMan);
        SpwSys.update(entityMan);
        CamSys.update(entityMan);
        
        /*std::cout << " [REN]: "  << timer.measureTimeToProcc([&](){ RenSys.update(entityMan); });
        std::cout << " [PHY]: "  << timer.measureTimeToProcc([&](){ PhySys.update(entityMan, 1.0/FPS); });
        std::cout << " [ANM]: "  << timer.measureTimeToProcc([&](){ AnmSys.update(entityMan); });
        std::cout << " [IN]: "   << timer.measureTimeToProcc([&](){ InpSys.update(entityMan); });
        std::cout << " [COLL]: " << timer.measureTimeToProcc([&](){ ColSys.update(entityMan); });
        std::cout << " [HTH]: "  << timer.measureTimeToProcc([&](){ HthSys.update(entityMan); });
        std::cout << " [SPW]: "  << timer.measureTimeToProcc([&](){ SpwSys.update(entityMan); });
        std::cout << " [CAM]: "  << timer.measureTimeToProcc([&](){ CamSys.update(entityMan); }) <<"\n\n";
    */
        timer.waitForUntil_ns(timePF);

        if (InpSys.isKeyPress(XK_Escape))      activeState = false;
        else if (InpSys.isKeyPress(XK_p))      stateMan.pushState<Pause_t>();
        else if (InpSys.isKeyPress(XK_d))      RenSys.setDebugDraw(debugDraw=!debugDraw); // Marcado de bounding box en las entidades (solo las que tienen collider de componente)
	}

	bool isActiveState() final override { return activeState; }

private:

	ECS::EntityManager_t entityMan { 800 };  // Manager of entities and components
    StateManager_t& stateMan;

    // SYSTEMS TO USE
    const RenderSys_t&    RenSys;
    const PhysicsSys_t&   PhySys;
    const InputSys_t&     InpSys;
    const CollisionSys_t& ColSys;
    const HealthSys_t&    HthSys;
    const SpawnSys_t&     SpwSys;
    const CameraSys_t&    CamSys;
    const AnimationSys_t& AnmSys;

    GameTimer_t timer {};
    bool activeState {true}, debugDraw {false};

};