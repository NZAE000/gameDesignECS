#pragma once
#include <cstdint>
#include <engine/cmp/component.hpp>
#include <engine/util/typeAliases.hpp>


struct PhysicsCmp_t : ECS::Component_t<PhysicsCmp_t> {

	explicit PhysicsCmp_t(ECS::EntityID_t eid) 
	: Component_t(eid) {}

	// UNITS MOVEMENTS PER SECONDS
	static constexpr float MAX_VX        { 280.0f     };
	static constexpr float STD_AX        { MAX_VX/0.2 };
	static constexpr float MINVX_BRAKING { 0.005f*MAX_VX }; // MINIMA VX PARA SABER QUE SE HA FRENADO APROXIMANDOSE A 0

	static constexpr float MAX_VY           { 400.0f        };
	static constexpr float GRAVITY          { MAX_VY/0.4    };
	static constexpr float MINVY_ONPLATFORM { 0.05f*GRAVITY }; // MINIMA VY PARA SABER QUE SE HA DE ESTAR EN PLATAFORMA

	// Non-type template parameter: passing a literal to template, to know the size of the array
	static constexpr std::array //<int32_t, 14> 
	JUMPS_PHASES = {-400,-400,-400,-350,-350,-350,-350,-300,-300,-300,-300,-200,-200,-200,-200,-100,-100,-100,-50,-50, -50};

	// When zero velocity is counted n times, then the entity is on the ground
	//static constexpr uint8_t TIMES_VY_ZERO { 3 };

	constexpr bool isJumpEnabled() const
	{
		// before implemented
		//return (jumpIndexPhase == JUMPS_PHASES.size() && countVyZero == TIMES_VY_ZERO);
		
		// NOW
		return (jumpIndexPhase == JUMPS_PHASES.size() && onPlatform);
	}

	constexpr void startJumpPhase() { jumpIndexPhase = 0; }
	constexpr bool isInJumpPhase()  { return (jumpIndexPhase < JUMPS_PHASES.size()); }

	constexpr void applyAccelerationX(double dt) { vx += ax*dt; }
	constexpr void applyAccelerationY(double dt) { vy += g*dt;  }

	// VARIABLES
	float  x { 0 },  y { 0 };    	// position
	float vx { 0 }, vy { 0 };   	// speed
	float ax { 0 };					// x acceleration (0 default)
	float  g { 0 };    	  		    // y acceleration (gravity) (0 default)
	float friction { 1.0f };        // rozamiento

	uint8_t jumpIndexPhase { JUMPS_PHASES.size() }; // last index: indicates that there is no jump
	bool onPlatform { false }; // OJO
	//uint8_t countVyZero	   { 0 };
};