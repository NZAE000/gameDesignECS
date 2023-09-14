#pragma once
#include <engine/util/keyBoard.hpp>
#include <engine/sys/system.hpp>

namespace ECS {
	struct EntityManager_t;
}

struct InputSys_t : ECS::System_t<InputSys_t> {

	explicit InputSys_t();

	void update(ECS::EntityManager_t&) const;
	bool isKeyPress(KeySym) const noexcept;
	void setOn() const noexcept;

private:

	inline static ECS::KeyBoard_t keyboard {};
	
	static void onKeyPress(KeySym)   noexcept;
	static void onKeyRelease(KeySym) noexcept;
};