#include "animationManager.hpp"

AnimManager_t::AnimManager_t()
{
	Appearance_t& player     = createAppearance(CHARAC_t::PLAYER, ACTION_t::DEFAULT, "./assets/deadpool.png");
	BoundingBNode& boxRootPl = player.boxRoot;

	// Set boundign boxes on my principal sprite player
    boxRootPl.box = { 0, 0, player.w-7, player.h }; // 1. bounding principal
    boxRootPl.subBoxes = {
        { { 11, 1, 37, 38 }, false, // 2. subbox
            {
                { { 16,  2, 28,  6 }, false, {} }, // 3. subbox
                { { 14,  8, 33, 22 }, false, {} }, // 3.
                { { 15, 30, 29,  8 }, false, {} }  // 3.
            } 
        },
        { { 1, 39, 47, 43 }, false, // 2.
            {
                { { 4,  40, 40, 10 }, false, {} }, // 3.
                { { 2,  50, 12, 31 }, false, {} }, // 3.
                { { 21, 50, 25, 31 }, false, {} }  // 3.
            } 
        }, 
        { { 3, 82, 43, 30 }, false, // 2.
            {}
        }
    };

    Appearance_t& blade      = createAppearance(CHARAC_t::BLADE, ACTION_t::DEFAULT, "./assets/blade.png");
    BoundingBNode& boxRootBl = blade.boxRoot;
    boxRootBl.box = { 5 ,5, blade.w-10, blade.h-10 };

    Appearance_t& platform     = createAppearance(CHARAC_t::PLATFORM, ACTION_t::DEFAULT, "./assets/platform.png");
	BoundingBNode& boxRootPtmf = platform.boxRoot;
	boxRootPtmf.box = { 0, 0, platform.w, platform.h };
}

Appearance_t* AnimManager_t::getAppearance(CHARAC_t charac, ACTION_t action)
{
	Appearance_t* appearance { nullptr };
	opIter_actions opItAction = findIteratorMapActions(charac);
	if (opItAction)
	{
		Actions& actions = *(*opItAction)->second.get();
		opIter_appearences opItAppear = findIteratorMapAppearances(actions, action);
		if (opItAppear) 
		{
			Appearances& appears = *(*opItAppear)->second.get();
			if (!appears.empty()) appearance = &appears.front(); 
		}
	}
	return appearance;
}


// ITERATORS

AnimManager_t::opIter_actions 
AnimManager_t::findIteratorMapActions(CHARAC_t charac) noexcept
{
	opIter_actions opIt = animations.find(charac);
	if (*opIt == animations.end()) opIt.reset();

	return opIt;
}

AnimManager_t::opIter_appearences
AnimManager_t::findIteratorMapAppearances(Actions& actions, ACTION_t action) noexcept
{
	opIter_appearences opIt = actions.find(action);
	if (*opIt == actions.end()) opIt.reset();

	return opIt;
}

//********************************************************************************************

Appearance_t&
AnimManager_t::createAppearance(CHARAC_t charac, ACTION_t action, std::string_view filename)
{
	Appearances* appears { nullptr };

	Actions& actions        = getActions(charac);
	opIter_appearences opIt = findIteratorMapAppearances(actions, action);

	if (opIt) appears = (*opIt)->second.get();
	else      appears = &createAppearances(actions, action);

	return appears->emplace_back(filename);
}


AnimManager_t::Actions& AnimManager_t::getActions(CHARAC_t charac)
{
	Actions* actions { nullptr };
	opIter_actions opIt = findIteratorMapActions(charac);

	if (opIt) actions = (*opIt)->second.get();
	else      actions = &createAnimations(charac);

	return *actions;
}

AnimManager_t::Actions& AnimManager_t::createAnimations(CHARAC_t charac)
{
	ECS::UPTR<Actions> actions = std::make_unique<Actions>();
	auto* ptrActions   = actions.get();
	animations[charac] = std::move(actions); // move resource

	return *ptrActions;
}

AnimManager_t::Appearances& AnimManager_t::createAppearances(Actions& actions, ACTION_t action)
{
	ECS::UPTR<Appearances> appears = std::make_unique<Appearances>();
	appears->reserve(STDNUM_ACTIONS);
	auto* ptrAppears = appears.get();
	actions[action]  = std::move(appears);

	return *ptrAppears;
}