#pragma once

enum class CHARAC_t
{ 
	PLAYER, BLADE, PLATFORM, SPAWNER
};

enum class ACTION_t
{
	DEFAULT, MOVE_SIDE, JUMP, JUMP_SIDE, DUCK, PUNCH, KICK
};

using FRAME_SEQUENCE = std::size_t;
using REP_PER_FRAME  = std::size_t;