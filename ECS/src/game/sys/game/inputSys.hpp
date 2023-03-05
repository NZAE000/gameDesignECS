#pragma once
#include <ecs/util/keyBoard.hpp>
#include <ecs/sys/system.hpp>

struct InputSys_t : ECS::SystemBase_t<InputSys_t> {

	explicit InputSys_t();

	void update(ECS::EntityManager_t&) const;
	bool isKeyPress(KeySym) const noexcept;
	void setOn() const noexcept;

private:

	inline static ECS::KeyBoard_t keyboard {};

	static void onKeyPress(KeySym)   noexcept;
	static void onKeyRelease(KeySym) noexcept;
};