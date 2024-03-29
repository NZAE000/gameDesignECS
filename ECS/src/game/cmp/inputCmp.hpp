#pragma once
#include <engine/cmp/component.hpp>
#include <engine/util/typeAliases.hpp>
#include <functional>
#include <X11/X.h>
#include <X11/keysym.h>

struct PhysicsCmp_t; // forward declaration

struct InputCmp_t : ECS::Component_t<InputCmp_t> {

	explicit InputCmp_t (ECS::EntityID_t eid)
	: Component_t(eid) 
	{};

	template<typename CALLABLE>
	constexpr void addAction(KeySym key, CALLABLE action) // operator [] cant throw excpt.
	{
		actions[key] = action;
	}

	constexpr const auto& getActions() const noexcept { return actions; }

	// Mapping to key - action
	ECS::Hash_t<KeySym, std::function<void(PhysicsCmp_t&)>> actions;

	/*KeySym key_UP    { XK_Up    }; 
	KeySym key_DOWN  { XK_Down  }; 
	KeySym key_LEFT  { XK_Left  }; 
	KeySym key_RIGHT { XK_Right };*/

};
