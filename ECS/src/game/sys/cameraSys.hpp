#pragma once

template<typename GameCTX_t>
struct CameraSys_t {

	explicit CameraSys_t() = default;

	void update(GameCTX_t&) const noexcept;

};