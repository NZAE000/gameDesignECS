#pragma once
#include <cstdint>

template<typename Type>
struct Box_t {

	constexpr Type getXLeft() const noexcept  { return x;         }
	constexpr Type getXRight() const noexcept { return x + (w-1); }

	constexpr Type getYUp()   const noexcept  { return y;         }
	constexpr Type getYDown() const noexcept  { return y + (h-1); }

	Type x {}, y {};
	Type w {}, h {};
};