#pragma once
#include <ecs/util/typeAliases.hpp>
#include <ecs/cmp/component.hpp>
#include <string_view>
#include <cstdint>


struct RenderCmp_t : ECS::ComponentBase_t<RenderCmp_t> {

    explicit RenderCmp_t(ECS::EntityID_t eid)
    : ComponentBase_t(eid) 
    { }

    void loadFromPng(const std::string_view filename);

    uint32_t w{0}, h {0};                // dimension
    std::vector<uint32_t> sprite {};     // pixeles del personaje

private:
	auto loadPngIntoVector(const std::string_view filename);
	void initSpriteFromABGRData(const std::vector<unsigned char>& pixels);

};
