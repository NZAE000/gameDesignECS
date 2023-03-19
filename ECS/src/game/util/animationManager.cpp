#include "animationManager.hpp"

AnimManager_t::AnimManager_t()
{
	// INTIALIZE ALL ANIMATIONS

	Appearance_t& playerStand1 = createAppearance(CHARAC_t::PLAYER, ACTION_t::DEFAULT, "./assets/dpstand-3.png");
	BoundingBNode& boxRootPl   = playerStand1.boxRoot;

	// Set boundign boxes on my principal sprite playerStand1
    boxRootPl.box = { 0, 0, playerStand1.w, playerStand1.h }; // 1. bounding principal
    boxRootPl.subBoxes = {
    	{ { 37, 1, 22, 17 }, false, // HEAD
            {
            	{ { 39, 3, 15, 5 }, false, {} },
            	{ { 38, 8, 20, 9 }, false, {} }
            } 
        },
    	{ { 11, 18, 68, 29 }, false, // 2. TORSO - ARMS
            {
            	{ { 12, 19, 23, 27 }, false, // ARM 1
            		{
            			{
            				{ { 21, 20, 13, 10}, false, {} }, // shoulder
            				{ { 14, 30, 20, 15}, false, {} }  // forearm-hand
            			}
            		} 
            	}, 
            	{ { 35, 19, 19, 27}, false, {} },  // TORSO
            	{ { 54, 31, 24, 14 }, false, {} }  // AMR 2
            } 
        },
    	{ { 28, 47, 30, 21 }, false, // 2. PELVIS
            {
            } 
        },
    	{ { 8, 68, 28, 28 }, false, // 2. LEG 1
            {
            	{ { 21, 69, 14, 9  }, false, {} }, // thigh
            	{ { 10, 80, 15, 15 }, false, {} }  // knee
            } 
        },
    	{ { 52, 68, 16, 28 }, false, // 2. LEG 2
            {
            	{ { 53, 69, 12, 11 }, false, {} }, // thigh
            	{ { 55, 80, 12, 15 }, false, {} }  // knee
            } 
        },
        { { 1, 96, 14, 13 }, false, // 2. FEET 1
            {
            } 
        },
        { { 58, 96, 21, 13 }, false, // 2. FEET 2
            {
            } 
        }
    };

    Appearance_t& blade      = createAppearance(CHARAC_t::BLADE, ACTION_t::DEFAULT, "./assets/blade.png");
    BoundingBNode& boxRootBl = blade.boxRoot;
    boxRootBl.box = { 5 ,5, blade.w-10, blade.h-10 };

    Appearance_t& platform     = createAppearance(CHARAC_t::PLATFORM, ACTION_t::DEFAULT, "./assets/platform.png");
	BoundingBNode& boxRootPtmf = platform.boxRoot;
	boxRootPtmf.box = { 0, 0, platform.w, platform.h };

	Appearance_t& spawner     = createAppearance(CHARAC_t::SPAWNER, ACTION_t::DEFAULT, "./assets/spawner.png");
	BoundingBNode& boxRootSpw = spawner.boxRoot;
	boxRootSpw.box = { 0, 0, 5, 5 };
}

const Appearance_t* AnimManager_t::getAppearance(CHARAC_t charac, ACTION_t action) const noexcept
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

Appearance_t* AnimManager_t::getAppearance(CHARAC_t charac, ACTION_t action) noexcept
{
	const Appearance_t* appearance = const_cast<const AnimManager_t*>(this)->getAppearance(charac, action);
	return const_cast<Appearance_t*>(appearance);
}

// ITERATORS *******************************************************************************

AnimManager_t::opIter_actions 
AnimManager_t::findIteratorMapActions(CHARAC_t charac) const noexcept
{
	opIter_actions opIt = animations.find(charac);
	if (*opIt == animations.end()) opIt.reset();

	return opIt;
}

AnimManager_t::opIter_appearences
AnimManager_t::findIteratorMapAppearances(const Actions& actions, ACTION_t action) const noexcept
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

	Actions& actions        = getActions(charac); // Get map action-appearances. If not exist, is created.
	opIter_appearences opIt = findIteratorMapAppearances(actions, action); // Get vector appearances.

	if (opIt) appears = (*opIt)->second.get();
	else      appears = &createAppearances(actions, action); // IF THE ACTION IS NOT, IS CREATED.

	return appears->emplace_back(filename); // The appearance is created and is returned.
}


// Get map action-appearences. If not exist, is created.
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