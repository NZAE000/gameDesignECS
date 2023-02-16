#pragma once
#include <cstdint>
#include <ecs/cmp/component.hpp>
#include <ecs/util/typeAliases.hpp>


struct PhysicsCmp_t : ECS::ComponentBase_t<PhysicsCmp_t> {

	explicit PhysicsCmp_t(ECS::EntityID_t eid) 
	: ComponentBase_t(eid) {}

	// Gravity constants
	static constexpr int32_t GRAVITY {  1 };
	static constexpr int32_t MIN_VY  { -6 };
	static constexpr int32_t MAX_VY  {  6 };
	static constexpr int32_t MIN_VX  { -4 };
	static constexpr int32_t MAX_VX  {  4 };

	// Non-type template parameter: passing a literal to template, to know the size of the array
	static constexpr std::array <int32_t, 14> 
	JUMPS_PHASES = {-7, -7, -7, -6, -6, -6, -6, -5, -5, -5, -4, -4, -3, -3}; 

	// When zero velocity is counted n times, then the entity is on the ground
	static constexpr uint8_t TIMES_VY_ZERO { 3 };

	constexpr bool isJumpEnabled() const
	{
		return (jumpIndexPhase == JUMPS_PHASES.size() && countVyZero == TIMES_VY_ZERO);
	}

	constexpr void startJumpPhase() { jumpIndexPhase = 0; }
	constexpr bool isInJumpPhase()  { return (jumpIndexPhase < JUMPS_PHASES.size()); }
	constexpr void applyAccelerationY() { vy += g; }

	// VARIABLES
	uint32_t x { 0 }, y  { 0 };    	// position
	int32_t vx { 1 }, vy { 1 };   	// speed
	int32_t g  { 0 };    	  		// gravity (0 default)

	uint8_t jumpIndexPhase { JUMPS_PHASES.size() }; // last index: indicates that there is no jump
	uint8_t countVyZero	   { 0 };
};