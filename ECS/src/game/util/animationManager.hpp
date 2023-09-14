#pragma once
#include "appearance.hpp"
#include "typeAnimations.hpp"
#include <engine/util/typeAliases.hpp>
#include <optional>


struct AppearFrames_t {
	explicit AppearFrames_t(std::string_view filename, const REP_PER_FRAME& r)
	: appear{filename}, rep{r} {}

	explicit AppearFrames_t() {}

	Appearance_t appear;
	REP_PER_FRAME  rep;
};

// SINGLETON MANAGER OF ALL ANIMATIONS of game characters and with his actions
struct AnimManager_t {

	static constexpr uint8_t STDNUM_FRAMES { 10 };

	using Appearances        = ECS::Vec_t<AppearFrames_t>;
	using Actions        	 = ECS::Hash_t<ACTION_t, ECS::UPTR_t<Appearances>>;
	using opIter_actions 	 = std::optional<ECS::Hash_t<CHARAC_t, ECS::UPTR_t<Actions>>::const_iterator>;
	using opIter_appearences = std::optional<Actions::const_iterator>;

	const AppearFrames_t* getAppearance(CHARAC_t, ACTION_t, FRAME_SEQUENCE&) const noexcept;
	      AppearFrames_t* getAppearance(CHARAC_t, ACTION_t, FRAME_SEQUENCE&)       noexcept;

	void onNormalAnimations()   noexcept { currentMapAnim = &animations;     }
	void onFlippingAnimations() noexcept { currentMapAnim = &flipAnimations; }

	static AnimManager_t& getInstanse()
	{
		static AnimManager_t animMan {};
		return animMan;
	}

private:

	explicit AnimManager_t();
	Appearance_t& createAppearance(CHARAC_t, ACTION_t, std::string_view filename, REP_PER_FRAME);
	void createFlipAppearances(CHARAC_t, ACTION_t);
	void flipAppearances(const Appearances& normalAppears, Appearances& flippedAppears) noexcept;
	void flipBoxTree(const BoundingBNode& normalTreeBox, BoundingBNode& flippedTreeBox, uint32_t width, uint32_t height) const noexcept;
	void flipSprite(const AppearFrames_t& normalAF, AppearFrames_t& flippedAF) const noexcept;
	Appearances& getAppearences(CHARAC_t, ACTION_t);
	opIter_actions findIteratorMapActions(CHARAC_t) const noexcept;
	opIter_appearences findIteratorMapAppearances(const Actions&, ACTION_t) const noexcept;
	Actions& getActions(CHARAC_t);
	Actions& createMapActions(CHARAC_t);
	Appearances& createAppearances(Actions&, ACTION_t);

	ECS::Hash_t<CHARAC_t, ECS::UPTR_t<Actions>> animations;		// to right
	ECS::Hash_t<CHARAC_t, ECS::UPTR_t<Actions>> flipAnimations;   // to left
	ECS::Hash_t<CHARAC_t, ECS::UPTR_t<Actions>>* currentMapAnim { &animations };
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