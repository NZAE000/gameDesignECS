#pragma once
#include <stack>
#include "state.hpp"
#include <memory>
#include <type_traits>

struct StateManager_t {

	explicit StateManager_t() = default;

	template<typename STATE, typename... PARAMS> 	// Type state and args types for constructor.
	void pushState(PARAMS&&... params) 				// With universal references (coulding be lvalues or rvalues)
	{
		static_assert(std::is_base_of<State_t, STATE>::value, "Error: Invalid state type.");  // Check if 'STATE type' is compatible with State_t on compilation time. With assert_static stop compilation.
		states.push(std::make_unique<STATE>(std::forward<decltype(params)>(params)...));      // forward: if params be lvalue, then pass lvalue reference, or if params rvalue, pass how rvalue reference.
	}

	void update() 
	{
		auto& statePriority { states.top() };

		if (statePriority && statePriority.get()->isActiveState())
			statePriority.get()->update();

		else states.pop();
	}

	bool thereAnyState() const { return !states.empty(); }

private:
	std::stack<std::unique_ptr<State_t>> states;
};