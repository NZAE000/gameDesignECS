#include <iostream>
#include <engine/man/systemManager.hpp>
#include <game/man/game.hpp>
#include <game/man/stateManager.hpp>
#include <game/util/goFactory.hpp>
#include <game/util/gameBuffer.hpp>
#include <game/sys/game/renderSys.hpp>
#include <game/sys/game/physicsSys.hpp>
#include <game/sys/game/collisionSys.hpp>
#include <game/sys/game/healthSys.hpp>
#include <game/sys/game/inputSys.hpp>
#include <game/sys/game/cameraSys.hpp>
#include <game/sys/game/spawnSys.hpp>
#include <game/sys/game/animationSys.hpp>
#include <game/util/animationManager.hpp>

static constexpr uint32_t WIDTH  { 640 };
static constexpr uint32_t HEIGHT { 360 };


struct Menu_t : State_t {

    explicit Menu_t(StateManager_t& sm) : stMan{sm} {}

    void update() final override
    {
        short op {};
        std::cout<<"\nGAME1!!: \n\n"
                 <<"1- PLAY\n"
                 <<"2- OPTIONS\n"
                 <<"3- EXIT\n";
        
        std::cin>>op;
        switch(op)
        {
        case 1:     stMan.pushState<GameMan_t>(stMan); break;
        case 2:     break;
        default:    activeState=false;
        }
    }

    bool isActiveState() final override { return activeState; }

private:
    bool activeState { true };
    StateManager_t& stMan;
};


int 
main(void)
try {
    /*uint16_t bin = 0b00000000;
    uint16_t bin2 = bin | static_cast<uint16_t>(std::pow(2, 0));
    std::cout<<bin2<<std::endl;

    bin2 = bin2 | static_cast<uint16_t>(std::pow(2, 1));
    bin2 = bin2 | static_cast<uint16_t>(std::pow(2, 7));

    ACTION_t action = static_cast<ACTION_t>(bin2);*/

    ECS::SystemManager_t SysManager {};                                  // SYSTEM MANAGER
    GOFactory_t          GOFactory  { AnimManager_t::getInstanse() };    // GAME OBJECT FACTORY 
    FrameBuffer_t        FrameBuff  { WIDTH, HEIGHT };                   // MY BUFFER FOR DRAW GAME OBJECTS (tinyptc)
    
    // INIT SYSTEMS
    SysManager.createSys<RenderSys_t>(FrameBuff);
    SysManager.createSys<InputSys_t>();
    SysManager.createSys<PhysicsSys_t>();
    SysManager.createSys<CollisionSys_t>(WIDTH, HEIGHT);
    SysManager.createSys<HealthSys_t>();
    SysManager.createSys<SpawnSys_t>();
    SysManager.createSys<CameraSys_t>();
    SysManager.createSys<AnimationSys_t>(AnimManager_t::getInstanse());

    StateManager_t stateMan   { SysManager, GOFactory };
    stateMan.pushState<Menu_t>(stateMan);

    while(stateMan.thereAnyState()) stateMan.update();

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
