#pragma once
#include <ecs/util/keyBoard.hpp>


template<typename GameCTX_t>
struct InputSys_t {

	explicit InputSys_t();

	bool update(GameCTX_t&) const;

private:

	inline static ECS::KeyBoard_t keyboard {};

	static void onKeyPress(KeySym);
	static void onKeyRelease(KeySym);
};