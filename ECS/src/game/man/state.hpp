#pragma once

// STATE INTERFACE!! 

struct State_t {

	virtual ~State_t() = default;

	virtual void update() = 0;
	virtual bool isActiveState() = 0;
};