#include <game/sys/game/inputSys.hpp>
#include <game/cmp/inputCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <ecs/man/entityManager.cpp>

extern "C" {
	#include <tinyPTC.ua/src/tinyptc.h>
}
#include <iostream>

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

		std::cout<<"x:"<<phycmp->x<<" y:"<<phycmp->y<<" vx: "<<phycmp->vx<<" vy: "<<phycmp->vy;
		phycmp->ax = 0; // only entities with input cmp

		const ECS::Hash_t<KeySym, std::function<void(PhysicsCmp_t&)>>& actions = input.getActions();
		ECS::Hash_t<KeySym, std::function<void(PhysicsCmp_t&)>>::const_iterator nextAction 
		= actions.begin();

		while(nextAction != actions.end())
		{
			if (keyboard.isKeyPress(nextAction->first))
				nextAction->second(*phycmp);
			++nextAction;
		}

		std::cout<<" ax: "<<phycmp->ax<<"\n";
		ptc_process_events();
	}
}