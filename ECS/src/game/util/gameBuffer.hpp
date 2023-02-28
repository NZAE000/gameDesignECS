#pragma once
#include <cstdint>
extern "C" {
	#include <tinyPTC.ua/src/tinyptc.h>
}


struct FrameBuffer_t {

	explicit FrameBuffer_t(uint32_t w, uint32_t h)
	: width{w}, height{h}, frameBuff { std::make_unique<uint32_t[]>(width*height) }
	{
		ptc_open("game1", width, height);
	}

	~FrameBuffer_t()    { ptc_close(); }

	void   update()    const { ptc_update(frameBuff.get()); }
	uint32_t* get() noexcept { return frameBuff.get(); }

	const uint32_t width{0}, height{0};

private:
	std::unique_ptr<uint32_t[]> frameBuff { nullptr };

};