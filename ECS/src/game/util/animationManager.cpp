#include "animationManager.hpp"

// INTIALIZE ALL ANIMATIONS
AnimManager_t::AnimManager_t()
{
	// CREATE DEFAULT ACTION PLAYER
	Appearance_t& playerStand1 	   = createAppearance(CHARAC_t::PLAYER, ACTION_t::DEFAULT, "./assets/dpstand-1.png", 12);
	Appearance_t& playerStand2	   = createAppearance(CHARAC_t::PLAYER, ACTION_t::DEFAULT, "./assets/dpstand-2.png", 12);
	Appearance_t& playerStand3	   = createAppearance(CHARAC_t::PLAYER, ACTION_t::DEFAULT, "./assets/dpstand-3.png", 12);
	BoundingBNode& boxRootPlStand1 = playerStand1.boxRoot;
	BoundingBNode& boxRootPlStand2 = playerStand2.boxRoot;
	BoundingBNode& boxRootPlStand3 = playerStand3.boxRoot;

	// SET BOUNDING BOXES playerStand1 

    boxRootPlStand1.box 	 = { 0, 0, playerStand1.w-1, playerStand1.h-1 };
    boxRootPlStand1.subBoxes = {
    	{ { 37, 1, 58, 17 }, false, // HEAD
            {
            	{ { 39, 3, 53,  7 }, false, {} },
            	{ { 38, 8, 57, 16 }, false, {} }
            } 
        },
    	{ { 11, 18, 78, 46 }, false, // 2. TORSO - ARMS
            {
            	{ { 12, 19, 34, 45 }, false, // ARM 1
            		{
            			{
            				{ { 21, 20, 33, 29}, false, {} }, // shoulder
            				{ { 14, 30, 33, 44}, false, {} }  // forearm-hand
            			}
            		}
            	}, 
            	{ { 35, 19, 53, 45}, false, {} },  // TORSO
            	{ { 54, 30, 77, 45 }, false, {} }  // AMR 2
            } 
        },
    	{ { 28, 47, 57, 67 }, false, // 2. PELVIS
            {
            } 
        },
    	{ { 8, 68, 35, 95 }, false, // 2. LEG 1
            {
            	{ { 21, 69, 34, 79  }, false, {} }, // thigh
            	{ { 10, 80, 24, 94 }, false, {} }  // knee
            } 
        },
    	{ { 52, 68, 67, 95 }, false, // 2. LEG 2
            {
            	{ { 53, 69, 64, 79 }, false, {} }, // thigh
            	{ { 55, 80, 66, 94 }, false, {} }  // knee
            } 
        },
        { { 1, 96, 14, 108 }, false, // 2. FEET 1
            {
            } 
        },
        { { 58, 96, 78, 108 }, false, // 2. FEET 2
            {
            } 
        }
    };

    boxRootPlStand2.box 	 = { 0, 0, playerStand2.w-1, playerStand2.h-1 };
	boxRootPlStand2.subBoxes = {
    	{ { 37, 1, 58, 17 }, false, // HEAD
            {
            	{ { 39, 3, 53,  7 }, false, {} },
            	{ { 38, 8, 57, 16 }, false, {} }
            } 
        },
    	{ { 11, 18, 78, 49 }, false, // 2. TORSO - ARMS
            {
            	{ { 12, 19, 34, 45 }, false, // ARM 1
            		{
            			{
            				{ { 21, 20, 33, 29}, false, {} }, // shoulder
            				{ { 14, 30, 33, 44}, false, {} }  // forearm-hand
            			}
            		}
            	}, 
            	{ { 35, 19, 53, 48}, false, {} },  // TORSO
            	{ { 54, 31, 77, 48 }, false, {} }  // AMR 2
            } 
        },
    	{ { 28, 50, 57, 67 }, false, // 2. PELVIS
            {
            } 
        },
    	{ { 8, 68, 35, 95 }, false, // 2. LEG 1
            {
            	{ { 21, 69, 34, 79  }, false, {} }, // thigh
            	{ { 10, 80, 24, 94 }, false, {} }  // knee
            } 
        },
    	{ { 52, 68, 67, 95 }, false, // 2. LEG 2
            {
            	{ { 53, 69, 64, 79 }, false, {} }, // thigh
            	{ { 55, 80, 66, 94 }, false, {} }  // knee
            } 
        },
        { { 1, 96, 14, 108 }, false, // 2. FEET 1
            {
            } 
        },
        { { 58, 96, 78, 108 }, false, // 2. FEET 2
            {
            } 
        }
    };

    boxRootPlStand3.box 	 = { 0, 0, playerStand3.w-1, playerStand3.h-1 };
	boxRootPlStand3.subBoxes = {
    	{ { 37, 1, 58, 17 }, false, // HEAD
            {
            	{ { 39, 3, 53,  7 }, false, {} },
            	{ { 38, 8, 57, 16 }, false, {} }
            } 
        },
    	{ { 12, 18, 76, 49 }, false, // 2. TORSO - ARMS
            {
            	{ { 13, 19, 37, 45 }, false, // ARM 1
            		{
            			{
            				{ { 22, 20, 36, 29}, false, {} }, // shoulder
            				{ { 15, 30, 36, 44}, false, {} }  // forearm-hand
            			}
            		}
            	}, 
            	{ { 38, 19, 53, 48}, false, {} },  // TORSO
            	{ { 54, 31, 75, 48 }, false, {} }  // AMR 2
            } 
        },
    	{ { 28, 50, 57, 67 }, false, // 2. PELVIS
            {
            } 
        },
    	{ { 8, 68, 35, 95 }, false, // 2. LEG 1
            {
            	{ { 21, 69, 34, 79  }, false, {} }, // thigh
            	{ { 10, 80, 24, 94 }, false, {} }  // knee
            } 
        },
    	{ { 52, 68, 67, 95 }, false, // 2. LEG 2
            {
            	{ { 53, 69, 64, 79 }, false, {} }, // thigh
            	{ { 55, 80, 66, 94 }, false, {} }  // knee
            } 
        },
        { { 1, 96, 14, 108 }, false, // 2. FEET 1
            {
            } 
        },
        { { 58, 96, 78, 108 }, false, // 2. FEET 2
            {
            } 
        }
    };

    // CREATE FLIP DEFAULT ACTION PLAYER (to left)
    createFlipAppearances(CHARAC_t::PLAYER, ACTION_t::DEFAULT);

    Appearance_t& blade      = createAppearance(CHARAC_t::BLADE, ACTION_t::DEFAULT, "./assets/blade.png", 1);
    BoundingBNode& boxRootBl = blade.boxRoot;
    boxRootBl.box = { 5 ,5, blade.w-5, blade.h-5 };

    Appearance_t& platform     = createAppearance(CHARAC_t::PLATFORM, ACTION_t::DEFAULT, "./assets/platform.png", 1);
	BoundingBNode& boxRootPtmf = platform.boxRoot;
	boxRootPtmf.box = { 0, 0, platform.w-1, platform.h-1 };

	Appearance_t& spawner     = createAppearance(CHARAC_t::SPAWNER, ACTION_t::DEFAULT, "./assets/spawner.png", 1);
	BoundingBNode& boxRootSpw = spawner.boxRoot;
	boxRootSpw.box = { 0, 0, 5, 5 };
}

void AnimManager_t::createFlipAppearances(CHARAC_t charac, ACTION_t action)
{
	currentMapAnim = &animations;
	// First check if exists the normal appearences
	opIter_actions opItAction = findIteratorMapActions(charac);
	if (opItAction)
	{
		Actions& actions = *(*opItAction)->second.get();
		opIter_appearences opItAppear = findIteratorMapAppearances(actions, action);
		if (opItAppear)
		{
			Appearances& normalAppears = *(*opItAppear)->second.get(); // appearences OKEY.

			// Then.. create the flipped appearences container (vector)
			currentMapAnim = &flipAnimations;
			Appearances* flippedAppears = &getAppearences(charac, action); // This should create new character-actions into flipAnimations
			if (!flippedAppears->size()) // Check if there are no flipped appearance frames
			{
				flipAppearances(normalAppears, *flippedAppears);
			}
		}
	}
	currentMapAnim = &animations;
}

void AnimManager_t::flipAppearances(const Appearances& normalAppears, Appearances& flippedAppears)
{
	for (const AppearFrames_t& normalAF : normalAppears)
	{
		const auto& normalSprite  = normalAF.appear.sprite;
		const auto& normalBoxRoot = normalAF.appear.boxRoot;
		const uint32_t width  	  = normalAF.appear.w;
		const uint32_t height 	  = normalAF.appear.h;
		const REP_PER_FRAME repFm = normalAF.rep;

		AppearFrames_t flippedAF;
		flippedAF.rep = repFm;
		flippedAF.appear.boxRoot = normalBoxRoot; // change NOW!!
		flippedAF.appear.sprite.resize(width*height);
		flippedAF.appear.w = width;
		flippedAF.appear.h = height;

		// Copy normal pixels flipped
		auto* ptrToNormalSprite  = normalAF.appear.sprite.data();
		auto* ptrToFlippedSprite = flippedAF.appear.sprite.data();
		for (uint32_t h=0; h<height; ++h)
		{
			ptrToNormalSprite += width-1;
			for (uint32_t w=0; w<width; ++w){
				*ptrToFlippedSprite = *ptrToNormalSprite;
				++ptrToFlippedSprite; --ptrToNormalSprite;
			}
			ptrToNormalSprite += width+1;
		}

		flippedAppears.push_back(flippedAF);
	}
}

const AppearFrames_t* AnimManager_t::getAppearance(CHARAC_t charac, ACTION_t action, FRAME_SEQUENCE& frameSec) const noexcept
{
	AppearFrames_t* appearance { nullptr };
	opIter_actions opItAction = findIteratorMapActions(charac);
	if (opItAction)
	{
		Actions& actions = *(*opItAction)->second.get();
		opIter_appearences opItAppear = findIteratorMapAppearances(actions, action);
		if (opItAppear)
		{
			Appearances& appears = *(*opItAppear)->second.get();
			if (!appears.empty()) 
			{
				if (frameSec >= appears.size()) frameSec = 0;
				appearance = &appears.at(frameSec);
			}
		}
	}
	return appearance;
}

AppearFrames_t* AnimManager_t::getAppearance(CHARAC_t charac, ACTION_t action, FRAME_SEQUENCE& frameSec) noexcept
{
	const AppearFrames_t* appearance = const_cast<const AnimManager_t*>(this)->getAppearance(charac, action, frameSec);
	return const_cast<AppearFrames_t*>(appearance);
}

// ITERATORS *******************************************************************************

AnimManager_t::opIter_actions 
AnimManager_t::findIteratorMapActions(CHARAC_t charac) const noexcept
{
	opIter_actions opIt = currentMapAnim->find(charac);
	if (*opIt == currentMapAnim->end()) opIt.reset();

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
AnimManager_t::createAppearance(CHARAC_t charac, ACTION_t action, std::string_view filename, REP_PER_FRAME rep)
{
	Appearances* appears         = &getAppearences(charac, action);
	AppearFrames_t& appearFrames = appears->emplace_back(filename, rep); // The appearance is created with his reps and is returned.
	
	return appearFrames.appear;
}

AnimManager_t::Appearances& 
AnimManager_t::getAppearences(CHARAC_t charac, ACTION_t action)
{
	Appearances* appears { nullptr };

	Actions& actions        = getActions(charac); // Get map action-appearances. If not exist, is created.
	opIter_appearences opIt = findIteratorMapAppearances(actions, action); // Get vector appearances.

	if (opIt) appears = (*opIt)->second.get();
	else      appears = &createAppearances(actions, action); // IF THE ACTION IS NOT, IS CREATED.

	return *appears;
}


// Get map action-appearences. If not exist, is created.
AnimManager_t::Actions& AnimManager_t::getActions(CHARAC_t charac)
{
	Actions* actions { nullptr };
	opIter_actions opIt = findIteratorMapActions(charac);

	if (opIt) actions = (*opIt)->second.get();
	else      actions = &createMapActions(charac);

	return *actions;
}

AnimManager_t::Actions& AnimManager_t::createMapActions(CHARAC_t charac)
{
	ECS::UPTR<Actions> actions = std::make_unique<Actions>();
	auto* ptrActions = actions.get();
	(*currentMapAnim)[charac] = std::move(actions); // move resource

	return *ptrActions;
}

AnimManager_t::Appearances& AnimManager_t::createAppearances(Actions& actions, ACTION_t action)
{
	ECS::UPTR<Appearances> appears = std::make_unique<Appearances>();
	appears->reserve(STDNUM_FRAMES);
	auto* ptrAppears = appears.get();
	actions[action]  = std::move(appears);

	return *ptrAppears;
}