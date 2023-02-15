#include <game/sys/inputSys.hpp>
#include <game/cmp/inputCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
extern "C" {
	#include <tinyPTC.ua/src/tinyptc.h>
}
#include <iostream>


template<typename GameCTX_t>
InputSys_t<GameCTX_t>::InputSys_t()
{
	// set pointer to func
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
		std::cout<<"VY Player: "<< phy.vy <<"\n";
		phy.vx = 0; // vy actua la gravedad y los saltos

		if ( keyboard.isKeyPress(input.key_LEFT) ) phy.vx = -1;
		if ( keyboard.isKeyPress(input.key_RIGHT)) phy.vx =  1;
		if ( keyboard.isKeyPress(input.key_UP)   ) 
		{
			if (phy.jumpIndexPhase == phy.JUMPS_PHASES.size() &&
				phy.countVyZero == phy.TIMES_VY_ZERO ){
				phy.jumpIndexPhase = 0;
			}
			//phy.vy = ;
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