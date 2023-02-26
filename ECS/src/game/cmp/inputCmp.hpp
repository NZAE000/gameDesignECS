#pragma once
#include <ecs/cmp/component.hpp>
#include <X11/X.h>
#include <X11/keysym.h>


struct InputCmp_t : ECS::ComponentBase_t<InputCmp_t> {

	explicit InputCmp_t (ECS::EntityID_t eid)
	: ComponentBase_t(eid) 
	{};

	KeySym key_UP    { XK_Up }; 
	KeySym key_DOWN  { XK_Down }; 
	KeySym key_LEFT  { XK_Left  }; 
	KeySym key_RIGHT { XK_Right };

};
