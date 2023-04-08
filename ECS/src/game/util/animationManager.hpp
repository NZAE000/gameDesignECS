#pragma once
#include "appearance.hpp"
#include <ecs/util/typeAliases.hpp>
#include <optional>

// SINGLETON MANAGER OF ALL ANIMATIONS of game characters and with his actions

enum class CHARAC_t
{ 
	PLAYER, BLADE, PLATFORM, SPAWNER
};

enum class ACTION_t
{
	DEFAULT, ADVANC_L, ADVANC_R, JUMP, DUCK, PUNCH, KICK
};

constexpr uint8_t STDNUM_ACTIONS { 10 };


struct AnimManager_t {

	using Appearances        = ECS::Vec_t<Appearance_t>;
	using Actions        	 = ECS::Hash_t<ACTION_t, ECS::UPTR<Appearances>>;
	using opIter_actions 	 = std::optional<ECS::Hash_t<CHARAC_t, ECS::UPTR<Actions>>::const_iterator>;
	using opIter_appearences = std::optional<Actions::const_iterator>;

	Appearance_t& createAppearance(CHARAC_t, ACTION_t, std::string_view filename);
	const Appearance_t* getAppearance(CHARAC_t, ACTION_t) const noexcept;
	      Appearance_t* getAppearance(CHARAC_t, ACTION_t)       noexcept;

	static AnimManager_t& getInstanse()
	{
		static AnimManager_t animMan {};
		return animMan;
	}

private:

	explicit AnimManager_t();
	opIter_actions findIteratorMapActions(CHARAC_t) const noexcept;
	opIter_appearences findIteratorMapAppearances(const Actions&, ACTION_t) const noexcept;
	Actions& createAnimations(CHARAC_t);
	Appearances& createAppearances(Actions&, ACTION_t);
	Actions& getActions(CHARAC_t);

	ECS::Hash_t<CHARAC_t, ECS::UPTR<Actions>> animations;
};

// ANIMATION CMP:
// ACTION (standing, advancing_left, advancing_right, jump, ducking, punch, kick, ...) 
// - FRAMES(sprites) PER ACTION (0,1,2,3, ..)

/*
IDEA:

ANIM_PLAYER:

 STANDING_ANIM:
 	appear01
 	appear02
 	appear03

 ADVANCING_LEFT:
 	appear01
 	appear02
 	appear03
	
 ADVANCING_RIGHT:
 	...
 	...
 	...
*/