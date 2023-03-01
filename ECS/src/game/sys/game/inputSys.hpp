#pragma once
#include <ecs/util/keyBoard.hpp>
#include <ecs/sys/system.hpp>

struct InputSys_t : ECS::SystemBase_t<InputSys_t> {

	explicit InputSys_t();

	void update(ECS::EntityManager_t&) const final override;
	void update(ECS::EntityManager_t&) final override {}
	bool isKeyPress(KeySym) const;
	void setOn() const;

private:

	inline static ECS::KeyBoard_t keyboard {};

	static void onKeyPress(KeySym);
	static void onKeyRelease(KeySym);
};