#include <iostream>
#include <thread>
#include <game/man/game.hpp>
#include <game/man/stateManager.hpp>
#include <game/util/gameBuffer.hpp>
#include <memory>

//static constexpr uint32_t WIDTH  { 640 };
//static constexpr uint32_t HEIGHT { 360 };

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

    StateManager_t stateMan   {};
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
