#include <game/sys/spawnSys.hpp>
#include <game/cmp/spawnCmp.hpp>
//#include <iostream>

struct RenderCmp_t;
struct ColliderCmp_t;


template<typename GameCTX_t>
void SpawnSys_t<GameCTX_t>::update(GameCTX_t& contx) const
{
	using clk = std::chrono::steady_clock;
	auto& spawncmps = contx.template getCmps<SpawnCmp_t>();

	clk::time_point now = clk::now();
	for (SpawnCmp_t& spwn : spawncmps)
	{
		auto* phycmp = contx.template getRequiredCmp<PhysicsCmp_t>(spwn);
		if (!phycmp) continue;

		auto delta_time { now - spwn.last_spawn };
		if (spwn.tobe_spawned > 0 && delta_time > spwn.spwan_interval)
		{
			spwn.last_spawn = now;
			spwn.spawnNow(spwn);
			//std::cout<<"new blade :D \n";
			--spwn.tobe_spawned;
		}
	}
}