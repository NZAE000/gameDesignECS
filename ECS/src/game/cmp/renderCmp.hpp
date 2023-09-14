#pragma once
#include <engine/util/typeAliases.hpp>
#include <engine/cmp/component.hpp>
#include <string_view>
#include <cstdint>


struct RenderCmp_t : ECS::Component_t<RenderCmp_t> {

    explicit RenderCmp_t(ECS::EntityID_t eid)
    : Component_t(eid) 
    { }

    uint32_t w{0}, h {0};             // dimension
    uint32_t* sprite { nullptr };     // pixeles del personaje
};
