#include <game/sys/game/inputSys.hpp>
#include <game/cmp/inputCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <ecs/man/entityManager.cpp>
extern "C" {
	#include <tinyPTC.ua/src/tinyptc.h>
}
//#include <iostream>

InputSys_t::InputSys_t() 
: SystemBase_t{} { setOn(); }

void InputSys_t::setOn() const noexcept
{
	// Set pointer to func
	ptc_set_on_keypress(onKeyPress);
	ptc_set_on_keyrelease(onKeyRelease);

	// Set all keys to realease
	keyboard.reset();
}

void InputSys_t::onKeyPress(KeySym key) noexcept
{
	keyboard.keyPressed(key);
}

void InputSys_t::onKeyRelease(KeySym key) noexcept
{
	keyboard.keyReleased(key);
}

bool InputSys_t::isKeyPress(KeySym key) const noexcept
{
	return keyboard.isKeyPress(key);
}

void InputSys_t::update(ECS::EntityManager_t& contx) const
{
	auto& inputCmps = contx.template getCmps<InputCmp_t>();
	for (auto& input : inputCmps) 
	{
		auto* phycmp = contx.template getRequiredCmp<PhysicsCmp_t>(input);
		if (!phycmp) continue;

		auto& phy = *(phycmp);
		//std::cout<<"x: "<<phy.x<<" vx: "<<phy.vx<<"\n";
		//std::cout<<"VY Player: "<< phy.vy <<" phase: " <<static_cast<uint32_t>(phy.jumpIndexPhase)<<" countvy0: "<<static_cast<uint32_t>(phy.countVyZero)<<"\n";
		phy.ax = 0; // only entities with input cmp

		ptc_process_events();
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
}