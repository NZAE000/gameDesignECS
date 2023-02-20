#include <game/sys/inputSys.hpp>
#include <game/cmp/inputCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
extern "C" {
	#include <tinyPTC.ua/src/tinyptc.h>
}
//#include <iostream>


template<typename GameCTX_t>
InputSys_t<GameCTX_t>::InputSys_t()
{
	// Set pointer to func
	ptc_set_on_keypress(onKeyPress);
	ptc_set_on_keyrelease(onKeyRelease);
}

template<typename GameCTX_t>
constexpr bool InputSys_t<GameCTX_t>::update(GameCTX_t& contx) const
{
	auto& inputCmps = contx.template getCmps<InputCmp_t>();

	for (auto& input : inputCmps) 
	{
		auto* phycmp = contx.template getRequiredCmp<PhysicsCmp_t>(input);
		if (!phycmp) continue;

		auto& phy = *(phycmp);
		//std::cout<<"y: "<<phy.y<<" VY: "<<phy.vy<<"\n";
		//std::cout<<"VY Player: "<< phy.vy <<" phase: " <<static_cast<uint32_t>(phy.jumpIndexPhase)<<" countvy0: "<<static_cast<uint32_t>(phy.countVyZero)<<"\n";
		phy.ax = 0; // only entitie with input cmp

		if ( keyboard.isKeyPress(input.key_LEFT) ) phy.ax = -phy.STD_AX;
		if ( keyboard.isKeyPress(input.key_RIGHT)) phy.ax =  phy.STD_AX;
		if ( keyboard.isKeyPress(input.key_UP)	 ) 
		{
			if (phy.isJumpEnabled()) {
				//std::cout<<"JUMP ENABLE\n";
				phy.startJumpPhase(); // Se habilita el salto iniciando su primera fase de salto.
			}
		}
		//if ( keyboard.isKeyPress(input.key_DOWN) ) phy.vy =  1;
	}

	return !ptc_process_events();
}

template<typename GameCTX_t>
void InputSys_t<GameCTX_t>::onKeyPress(KeySym key)
{
	keyboard.keyPressed(key);
}

template<typename GameCTX_t>
void InputSys_t<GameCTX_t>::onKeyRelease(KeySym key)
{
	keyboard.keyReleased(key);
}