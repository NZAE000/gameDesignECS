extern "C" {
    #include <tinyPTC.ua/src/tinyptc.h>
}
#include <game/sys/renderSys.hpp>
#include <game/cmp/renderCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <game/cmp/colliderCmp.hpp>
#include <algorithm>


template<typename GameCTX_t>
RenderSys_t<GameCTX_t>::RenderSys_t(uint32_t w, uint32_t h) 
    : widthScr {w}, heightScr {h}
    , frameBuffer { std::make_unique<uint32_t[]>(widthScr*heightScr) }
{   
    ptc_open("game", widthScr, heightScr);
}

template<typename GameCTX_t>
RenderSys_t<GameCTX_t>::~RenderSys_t(){ ptc_close(); }

template<typename GameCTX_t>
constexpr void RenderSys_t<GameCTX_t>::drawSpriteClipped(const RenderCmp_t& rencmp, const PhysicsCmp_t& phycmp) const
{

    // Clipping
    uint32_t left_off { 0 };
    uint32_t up_off   { 0 };
    // Drawing Coordinates and size
    uint32_t x { phycmp.x };
    uint32_t y { phycmp.y };
    uint32_t w { rencmp.w };
    uint32_t h { rencmp.h };

    // Horizontal clipping rules: cuando el sprite se pasa de los 4 lados/limites de screen

    if (x >= widthScr) {                 // Left clipping
        left_off = 0 - x;
        if (left_off >= w) return;      // Nothing to draw
        x = 0;
        w -= left_off;
    }
    else if (x + w >= widthScr) {       // Right clipping
        uint32_t right_off = x + w - widthScr;
        if (right_off >= w) return;     // Nothing to draw
        w -= right_off;
    }

    // Vertical clipping rules
    if (y >= heightScr) {               // Up clipping
        up_off = 0 - y;
        if (up_off > h) return;         // Nothing to draw
        y = 0;
        h -= up_off;
    }
    else if (y + h >= heightScr) {      // Down clipping
        uint32_t down_off = y + h - heightScr;
        if (down_off >= h) return;      // Nothing to draw
        h -= down_off;
    }
    
    // Render the entity
    auto ptr_toScr = getPosition(x, y);
    auto sprite_it = begin(rencmp.sprite) + up_off*rencmp.w + left_off;

    while(h--) 
    {
        for (uint32_t i=0; i<w; ++i) {
            // Draw only if transparency != 0 (not blending)
            if (*sprite_it & 0xFF000000) *ptr_toScr = *sprite_it;
            ++sprite_it;
            ++ptr_toScr;
        }
        sprite_it += rencmp.w - w;
        ptr_toScr += widthScr - w;
    }
}

template<typename GameCTX_t>
constexpr void RenderSys_t<GameCTX_t>::drawLineBox(uint32_t* ptr_toScr, uint32_t length, uint32_t displacement, uint32_t color) const
{
    while(length > 0) {
        *ptr_toScr = color;
        --length;
        ptr_toScr += displacement;
    }
}

template<typename GameCTX_t>
constexpr void RenderSys_t<GameCTX_t>::drawBox(const BoundingBox& box, uint32_t x, uint32_t y, uint32_t color) const
{
    // Coordinates sprite convertion to screen coordinates
    uint32_t xL { x + box.xLeft  };
    uint32_t xR { x + box.xRight };
    uint32_t yU { y + box.yUp    };
    uint32_t yD { y + box.yDown  };

    // Side sizes box
    uint32_t widthBox  = xR - xL;
    uint32_t heightBox = yD - yU;

    // up line
    drawLineBox(getPosition(xL, yU), widthBox, 1, color);
    // left line
    drawLineBox(getPosition(xL, yU), heightBox, widthScr, color);
    // right line
    drawLineBox(getPosition(xR-1, yU), heightBox, widthScr, color);
    // down line
    drawLineBox(getPosition(xL, yD-1), widthBox, 1, color);
}

template<typename GameCTX_t>
constexpr void RenderSys_t<GameCTX_t>::drawFillBox(const BoundingBox& box, uint32_t x, uint32_t y, uint32_t color) const
{
    // Coordinates sprite convertion to screen coordinates
    uint32_t xL { x + box.xLeft  };
    uint32_t xR { x + box.xRight };
    uint32_t yU { y + box.yUp    };
    uint32_t yD { y + box.yDown  };

    // Side sizes box
    uint32_t widthBox  = xR - xL;
    uint32_t heightBox = yD - yU;

    while (heightBox > 0){
        drawLineBox(getPosition(xL, yU), widthBox, 1, color);
        ++yU;
        --heightBox;
    }
}

template<typename GameCTX_t>
constexpr void RenderSys_t<GameCTX_t>::drawBoxTree(const BoundingBNode& treeBox, uint32_t x, uint32_t y, uint32_t color) const
{
    if (treeBox.isCollided)
        drawFillBox(treeBox.box, x, y, color); // when a box collide, then filled box whit his color.

    else drawBox(treeBox.box, x, y, color);

    for (const BoundingBNode& BBN : treeBox.subBoxes) drawBoxTree(BBN, x, y, color>>1);
}

template<typename GameCTX_t>
void RenderSys_t<GameCTX_t>::drawAll(const GameCTX_t& contx) const
{
    // Lamda Function
    // []  Captura: pasa todo lo que esta en contexto (arma una estructura con la captura). () = parámetros
    // [=] (por copia)
    // [&] (por referencia)
    // [&algo] (especifico)
    // por defecto trata la captura como cost. Para modificar, usar 'mutable'
    /*auto getPosition = [&](uint32_t x, uint32_t y){ // <- aca 
        return frameBuffer.get() + widthScr*y + x; // desplazarse tantas filas, luego desplazarlo en columna
    };*/
    
    auto& renderCmpts = contx.template getCmps<RenderCmp_t>();

    // Pintar todas las entidades, utilizando sus componentes de renderizado
    std::for_each(renderCmpts.begin(), renderCmpts.end(), [&](const RenderCmp_t& rendercmp){

        /*const auto* entity = contx.getEntityByID(rendercmp.getEntityID());
        if (!entity) return; // entity not found

        const auto* phycmp = entity->template getCmp<PhysicsCmp_t>();*/
        
        const auto* phycmp = contx.template getRequiredCmp<PhysicsCmp_t>(rendercmp);
        if (phycmp) 
        {
            drawSpriteClipped(rendercmp, *phycmp);

            // if debug is active, also render the bounding box 
            if (debugDraw) {
                auto* collcmp = contx.template getRequiredCmp<ColliderCmp_t>(*phycmp);
                if (!collcmp) return;

                //drawBox(collcmp->boxRoot.box, phycmp->x, phycmp->y, RED);
                drawBoxTree(collcmp->boxRoot, phycmp->x, phycmp->y, RED);

            }
        }
        /*{
            //auto& ent = *entity; // sutil
            auto ptr_toScr = getPosition(phycmp->x, phycmp->y);
            auto sprite_it = begin(rendercmp.sprite);
            for (uint32_t y=0; y<rendercmp.h; ++y)
            { 
                std::copy(sprite_it, sprite_it + rendercmp.w, ptr_toScr); // Copiar cada fila del sprite en una fila del screen
                sprite_it += rendercmp.w;
                ptr_toScr += widthScr;
            }
        }*/
        }
    );
}

template<typename GameCTX_t>
constexpr bool RenderSys_t<GameCTX_t>::update(GameCTX_t& contx) const
{
    const uint32_t size = widthScr*heightScr;
    auto screen         = frameBuffer.get();

    std::fill(screen, screen+size, BLACK);
    drawAll(contx);
    ptc_update(screen);

    return true;
}