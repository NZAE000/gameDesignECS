#pragma once
#include "state.hpp"
#include <stack>
#include <memory>
#include <type_traits>
#include <cstdint>
#include <ecs/man/systemManager.hpp>
#include <game/util/goFactory.hpp>

struct StateManager_t {

	explicit StateManager_t(ECS::SystemManager_t& sm, GOFactory_t& gf) 
	: sysMan{sm}, goFact{gf} {}

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

	template<typename SYS_t>
	constexpr SYS_t& getSys() { return sysMan.getSys<SYS_t>(); }

	template<typename SYS_t>
	constexpr const SYS_t& getSys() const { return sysMan.getSys<SYS_t>(); }

	void setManager(ECS::EntityManager_t& em) { goFact.setManager(em); }
	const auto& getFactory() const { return goFact; }
	auto& getFactory() 			   { return goFact; }
 
private:
	std::stack<std::unique_ptr<State_t>> states;
	ECS::SystemManager_t& sysMan;					// SYSTEM_MANAGER ref
	GOFactory_t& goFact;							// GAME OBJECT FACTORY ref
};