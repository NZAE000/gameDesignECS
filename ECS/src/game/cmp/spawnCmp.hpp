#pragma once
#include <ecs/cmp/component.hpp>
#include <chrono>
#include <functional>

using namespace std::chrono_literals;

struct SpawnCmp_t : ECS::ComponentBase_t<SpawnCmp_t> {

	using clk = std::chrono::steady_clock;

	explicit SpawnCmp_t(ECS::EntityID_t eid)
	: ComponentBase_t(eid) {}	


	// last spawn
	// spawn interval
	// count to be spawn

	clk::time_point               last_spawn	 { clk::now() };
	std::chrono::duration<double> spwan_interval { 3s };
	std::size_t					  tobe_spawned   { 2  };

	// Clase encapsuladora de un tipo de puntero a funcion, pero no literal void(*p)(void),
	// si no que apunta a cualquier cosa llamable que devuela aquel tipo reciva tal/es tipo/s.
	std::function<void(SpawnCmp_t)> spawnNow {};

};