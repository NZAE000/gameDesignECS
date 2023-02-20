#pragma once
#include <cstdint>
#include <ecs/cmp/component.hpp>
#include <ecs/util/typeAliases.hpp>


struct PhysicsCmp_t : ECS::ComponentBase_t<PhysicsCmp_t> {

	explicit PhysicsCmp_t(ECS::EntityID_t eid) 
	: ComponentBase_t(eid) {}

	// Gravity constants
	static constexpr float STD_AX  {  0.5f };
	static constexpr float MIN_VX  { -4.0f };
	static constexpr float MAX_VX  {  4.0f };

	static constexpr float GRAVITY {  0.5f };
	static constexpr float MIN_VY  { -6.0f };
	static constexpr float MAX_VY  {  6.0f };
	static constexpr float MINVY_ONPLATFORM { 2.5f*GRAVITY };

	// Non-type template parameter: passing a literal to template, to know the size of the array
	static constexpr std::array //<int32_t, 14> 
	JUMPS_PHASES = {-7,-7,-6,-6,-6,-6,-5,-5,-5,-4,-4,-3,-3};

	// When zero velocity is counted n times, then the entity is on the ground
	static constexpr uint8_t TIMES_VY_ZERO { 3 };

	constexpr bool isJumpEnabled() const
	{
		// before implemented
		//return (jumpIndexPhase == JUMPS_PHASES.size() && countVyZero == TIMES_VY_ZERO);
		
		// NOW
		return (jumpIndexPhase == JUMPS_PHASES.size() && onPlatform);
	}

	constexpr void startJumpPhase() { jumpIndexPhase = 0; }
	constexpr bool isInJumpPhase()  { return (jumpIndexPhase < JUMPS_PHASES.size()); }
	constexpr void applyAccelerationX() { vx += ax; }
	constexpr void applyAccelerationY() { vy += g;  }

	// VARIABLES
	float  x { 0 },  y { 0 };    	// position
	float vx { 0 }, vy { 0 };   	// speed
	float ax { 0 };					// x acceleration
	float g  { 0 };    	  		    // y acceleration (gravity) (0 default)
	float friction { 1.0f };        // rozamiento

	uint8_t jumpIndexPhase { JUMPS_PHASES.size() }; // last index: indicates that there is no jump
	bool onPlatform { true }; // OJO
	//uint8_t countVyZero	   { 0 };
};