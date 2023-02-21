#include <game/sys/cameraSys.hpp>
#include <game/cmp/cameraCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <game/cmp/renderCmp.hpp>

template<typename GameCTX_t>
void CameraSys_t<GameCTX_t>::update(GameCTX_t& contx) const noexcept
{
	auto& camcmps = contx.template getCmps<CameraCmp_t>();

	for(auto& camcmp : camcmps)
	{
		auto* phycmpCam = contx.template getRequiredCmp<PhysicsCmp_t>(camcmp);
		if (!phycmpCam) continue;

		auto* followEntity = contx.getEntityByID(camcmp.followEntID);
		if (!followEntity) continue;

		auto* phycmpEnt = followEntity->template getCmp<PhysicsCmp_t>();
		if (!phycmpEnt) continue;

		auto* rencmpEnt = followEntity->template getCmp<RenderCmp_t>();
		if (!rencmpEnt) continue;

		phycmpCam->x = phycmpEnt->x - (camcmp.width  - rencmpEnt->w)/2; 
		phycmpCam->y = phycmpEnt->y - (camcmp.height - rencmpEnt->h)/2;
	}
}