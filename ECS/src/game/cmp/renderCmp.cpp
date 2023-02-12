#include <game/cmp/renderCmp.hpp>
#include <picoPNG.ua/src/picopng.hpp>
#include <fstream>
#include <tuple>

void RenderCmp_t::initSpriteFromABGRData(const ECS::Vec_t<unsigned char>& pixels)
{
	sprite.reserve(pixels.size()/4);

    // Ahora se debe almacenar pixels uchar en mi vector uint32_t sprite de entity.
    // Coger cada uno de los 4 bytes y desplazarlos a su posicion dentro de los 32 bits de un uint32_t
    // para que esten en el formato correcto.
    for (auto px = pixels.begin(); px != pixels.end(); px+=4)
    {
        uint32_t pixel = 
            static_cast<uint32_t>(*(px+0)) << 16
        |   static_cast<uint32_t>(*(px+1)) <<  8 
        |   static_cast<uint32_t>(*(px+2))
        |   static_cast<uint32_t>(*(px+3)) << 24;
        sprite.emplace_back(pixel);
    }
    //std::memcpy(sprite.data(), pixels.data(), pixels.size());
    //sprite = std::vector<uint32_t>(pixels.begin(), pixels.end());
}

// auto return type: deduce los tipos, ya que ya se declaran o se conocen en la función.
auto RenderCmp_t::loadPngIntoVector(const std::string_view filename)
{
	std::vector<unsigned char> pixels {};
    unsigned long width_dec {0}, height_dec {0};
    std::ifstream file(filename.data(), std::ios::binary); // leer fichero en formato binario
    std::vector<unsigned char> fileContainer(
           std::istreambuf_iterator<char> {file} // iterador al principio
         , std::istreambuf_iterator<char> {}     // iterador al final
     );

    // La decodificación del png a uchar se almacenará en pixels.
    decodePNG(pixels, width_dec, height_dec, fileContainer.data(), fileContainer.size());

    //return std::tuple<std::vector<unsigned char>, unsigned long, unsigned long> {pixels, width_dec, height_dec}; 
    return std::tuple { pixels, width_dec, height_dec }; // CTAD: class template argument deduction:: since C++17.
}

void RenderCmp_t::loadFromPng(const std::string_view filename)
{
	// structure bindings: since C++17
    auto [pixels, width, height] = loadPngIntoVector(filename);

    initSpriteFromABGRData(pixels);
    
    w = width;
    h = height;
}