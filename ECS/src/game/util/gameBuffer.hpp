#pragma once
#include <cstdint>
#include <memory>
extern "C" {
	#include <tinyPTC.ua/src/tinyptc.h>
}


struct FrameBuffer_t {

	explicit FrameBuffer_t(uint32_t w, uint32_t h)
	: width{w}, height{h}, frameBuff { std::make_unique<uint32_t[]>(width*height) }
	{
		ptc_open("game1", width, height);
	}

	~FrameBuffer_t() { ptc_close(); }

	// Util methods 
	uint32_t* get() const noexcept { return frameBuff.get(); }
	void update()   const noexcept { ptc_update(frameBuff.get()); }
	void fill(uint32_t color) noexcept;
	void drawSprite(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t pixels_off, const uint32_t* ptrToSprite) noexcept;
	void drawAlignedLine(uint32_t x, uint32_t y, uint32_t lengthLine, uint32_t displacement, uint32_t color) noexcept;
	void drawFillRectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) noexcept;

	// Dimension
	const uint32_t width{0}, height{0};

private:

	// Helper functions member
	uint32_t* getPosition(uint32_t x, uint32_t y) const noexcept;

	std::unique_ptr<uint32_t[]> frameBuff { nullptr };

};