#pragma once

enum class CHARAC_t
{ 
	PLAYER, BLADE, PLATFORM, SPAWNER
};

enum class ACTION_t
{
	DEFAULT, ADVANC_L, ADVANC_R, JUMP, DUCK, PUNCH, KICK
};

using FRAME_SEQUENCE = std::size_t;
using REP_PER_FRAME  = std::size_t;