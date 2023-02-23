extern "C" {
    #include <tinyPTC.ua/src/tinyptc.h>
}
#include <game/sys/renderSys.hpp>
#include <game/cmp/renderCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <game/cmp/colliderCmp.hpp>
#include <game/cmp/cameraCmp.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>

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
constexpr void RenderSys_t<GameCTX_t>::drawSpriteClipped(const RenderCmp_t& rencmp, const PhysicsCmp_t& phycmp) const noexcept
{
    // CURRENT IMPLEMETATION (camera added)
    // COORDINATES TRANSFORMATION:

    //   SPRT     -->     WORLD      -->      CAMERA      -->     SCREEN
    //     (0,0 + POSSPR)       (-CAMPOSWRLD)         (+CAMPOSSCR)

    // Verify and get current camera components
    if (!currentCam.camcmp || !currentCam.phycmp) return;
    auto& camCmp      = *currentCam.camcmp;
    auto& phyCmpOfCam = *currentCam.phycmp;

    struct SprRefTo {   // Sprite coordinates reference to ..

        BoundingBox<float> world  {};
        BoundingBox<float> camera {};
        
        struct CamWithClipping {
            uint32_t left_off { 0 }, right_off { 0 };
            uint32_t up_off   { 0 }, down_off { 0 };
        } camWithClipping {};

        struct Screen {
            uint32_t x {}, y {};    // coordinates ref to screen
            uint32_t w {}, h {};    // new sprite dimensions (smaller or equal)
        } screen{};

    } sprRef {};

    // TRANSFORM SPRITE COORDINATES INTO 'WORLD' REF (0,0 + POSSPRITE)
    sprRef.world = {       
            phycmp.x                  // left
        ,   phycmp.x + rencmp.w       // right  
        ,   phycmp.y                  // up
        ,   phycmp.y + rencmp.h       // down
    };
    // TRANSFORM SPRITE COORDINATES INTO 'CAMERA' REF (POSSPRITEWRLD - POSCAMWRLD)
    sprRef.camera = {
            sprRef.world.xLeft  - phyCmpOfCam.x
        ,   sprRef.world.xRight - phyCmpOfCam.x
        ,   sprRef.world.yUp    - phyCmpOfCam.y
        ,   sprRef.world.yDown  - phyCmpOfCam.y
    };

    // Check the coordinates of the sprite referring to the camera is out of bounds of camera.
    if (   sprRef.camera.xLeft >= camCmp.width  || sprRef.camera.xRight < 0
        || sprRef.camera.yUp   >= camCmp.height || sprRef.camera.yDown  < 0 ) return;

    // Maybe only part of the sprite is in the camera..
    sprRef.camWithClipping = {
            static_cast<uint32_t>(std::round((sprRef.camera.xLeft  < 0             )?  -sprRef.camera.xLeft              : 0)) // p.ejem: si left esta a -3px en x de la camara, se debe recortar 3 pixeles.
        ,   static_cast<uint32_t>(std::round((sprRef.camera.xRight >= camCmp.width )?  sprRef.camera.xRight-camCmp.width : 0))
        ,   static_cast<uint32_t>(std::round((sprRef.camera.yUp    < 0             )?  -sprRef.camera.yUp                : 0))
        ,   static_cast<uint32_t>(std::round((sprRef.camera.yDown  >= camCmp.height)?  sprRef.camera.yDown-camCmp.height : 0))
    };

    // TRANSFORM SPRITE COORDINATES INTO SCREEN REF (sprite refcam coord + cam refscreen coord) (clipped) AND NEW DIMENSIONS
    sprRef.screen = {
            camCmp.xScr + static_cast<uint32_t>(std::round(sprRef.camera.xLeft) + sprRef.camWithClipping.left_off)     // x
        ,   camCmp.yScr + static_cast<uint32_t>(std::round(sprRef.camera.yUp)   + sprRef.camWithClipping.up_off)      // y
        ,   rencmp.w    - (sprRef.camWithClipping.left_off + sprRef.camWithClipping.right_off)                            // w
        ,   rencmp.h    - (sprRef.camWithClipping.up_off + sprRef.camWithClipping.down_off)                               // h
    };

    /*if (phycmp.getEntityID() == 1) { // show player coords
        std::cout<<"\ncamXscr: "<<camCmp.xScr<<" camYscr: "<<camCmp.yScr<<"\n";
        std::cout<<"camXwrld: "<<phyCmpOfCam.x<<" camYwrld: "<<phyCmpOfCam.y<<"\n";
        std::cout<<"xrefWrld: "<<phycmp.x<<" yrefWrld: "<<phycmp.y<<"\n";
        std::cout<<"xrefCam: "<<sprRef.camera.xLeft<<" yrefCam: "<<sprRef.camera.yUp<<"\n";
        std::cout<<"xrefScr: "<<sprRef.screen.x<<" yrefScr: "<<sprRef.screen.y<<"\n";
    }*/

    // RENDER BOUNDS OF CAM
    drawLineBox(getPosition(camCmp.xScr, camCmp.yScr), camCmp.height, widthScr, RED);                // left
    drawLineBox(getPosition(camCmp.xScr+camCmp.width-1, camCmp.yScr), camCmp.height, widthScr, RED); // right
    drawLineBox(getPosition(camCmp.xScr, camCmp.yScr), camCmp.width, 1, RED);                       // up
    drawLineBox(getPosition(camCmp.xScr, camCmp.yScr+camCmp.height-1), camCmp.width, 1, RED);       // down

    // RENDER SPRITE
    auto* ptr_toScr = getPosition(sprRef.screen.x, sprRef.screen.y);
    auto sprite_it  = begin(rencmp.sprite) + sprRef.camWithClipping.up_off*rencmp.w + sprRef.camWithClipping.left_off;

    while(sprRef.screen.h--) 
    {
        for (uint32_t i=0; i<sprRef.screen.w; ++i){
            // Draw only if transparency != 0 (not blending)
            if (*sprite_it & 0xFF000000) *ptr_toScr = *sprite_it;
            ++sprite_it;
            ++ptr_toScr;
        }
        sprite_it += rencmp.w - sprRef.screen.w;
        ptr_toScr += widthScr - sprRef.screen.w;
    }

    /*//BEFORE IMPLEMENTED!!!
    // Clipping
    uint32_t left_off { 0 };
    uint32_t up_off   { 0 };

    // Drawing Coordinates and size
    // OJO!! ARM
    uint32_t xSpr { 
        (phycmp.x >= 0)? static_cast<uint32_t>(std::round(phycmp.x)) 
        : -static_cast<uint32_t>(std::abs(std::round(phycmp.x)))                  
    };
    uint32_t ySpr { 
        (phycmp.y >= 0)? static_cast<uint32_t>(std::round(phycmp.y)) 
        : -static_cast<uint32_t>(std::abs(std::round(phycmp.y))) 
    };

    uint32_t wSpr { rencmp.w };
    uint32_t hSpr { rencmp.h };

    //std::cout<<"x: "<<xSpr<<" y: "<<ySpr<<"\n";

    // Horizontal clipping rules: cuando el sprite se pasa de los 2 lados/limites de screen 
    // (sucede cuando se le asigna un bounding box con ancho de menor dimension e interno al sprite).

    // Left clipping: cuando la posicion relativa al screen del scprite se pasa del limite izquiedo,
    // sucede que aquel numero de posicion será un valor muy grande (ya que las posiciones son valores sin signo,
    // p.ejem: 0 - 1 = 42949672965, por lo que se daria la vuelta a su valor maximo de un uint32_t).
    if (xSpr > widthScr){                
        left_off = 0 - xSpr;               // Ejm: si xSpr esta a -3 px del limite izquierdo, entonces xSpr = 42949672963. Por lo que si se resta 0 - 42949672963 se da la vuelta y da 3.
        if (left_off >= wSpr) return;      // Nothing to draw
        wSpr -= left_off;                  // Del ejemplo, el ancho del sprite se debe recortar 3 pixeles.
        xSpr = 0;                          // Como se paso del limite izquierdo, ya no se parte pintando de una posiscion fuera del limite, sino dellimite minimo 0 del screen.
    }
    // Right clipping: recordar que la posicion x,y del sprite se encuntra en la esquina superior izquierda del sprite,
    // por lo tanto, para saber si se ah pasado del limite derecho, a la posicion x se debe sumar el ancho del sprite, y comparar si es mayo al ancho del screeen.
    else if (xSpr + wSpr >= widthScr){
        uint32_t right_off = xSpr + wSpr - widthScr; // Ejm: si x esta a 630px y w del sprite es 50px, y el ancho del screen es 640, entonces la posicion mas el ancho del sprite daria 680px, por lo que es > a 640 y se estaria pasando 40px.
        if (right_off >= wSpr) return;     // Nothing to draw
        wSpr -= right_off;                 // Del ejemplo, el ancho del sprite se debe recortar 40 pixeles, quedando con 10px de ancho para dibujar.  
        // La posicion x no cambia, ya que sigue estando en el rango del ancho de screen.                                    
    }

    // Vertical clipping rules: cuando el sprite se pasa del limite inferior o superior del screen (analogo con lo que sucede en los limites izquierdo y derecho).
    // (sucede cuando se le asigna un bounding box con alto de menor dimension e interno al sprite).

    // Up clipping
    if (ySpr > heightScr){
        up_off = 0 - ySpr;
        if (up_off >= hSpr) return;        // Nothing to draw
        hSpr -= up_off;
        ySpr  = 0;
    }
    // Down clipping
    else if (ySpr + hSpr >= heightScr){
        uint32_t down_off = ySpr + hSpr - heightScr;
        if (down_off >= hSpr) return;      // Nothing to draw
        hSpr -= down_off;
    }
    
    // Render the sprite of entity
       //Si se paso del lim.izqu, x=0,
       //si se paso del lim.dere, x se mantiene, 
       //si se paso del lim.sup, y=0,
       //si se paso del lim.inf, y se mantiene
    auto* ptr_toScr = getPosition(xSpr, ySpr);
    auto sprite_it  = begin(rencmp.sprite) + up_off*rencmp.w + left_off;

    while(hSpr--) 
    {
        for (uint32_t i=0; i<wSpr; ++i){
            // Draw only if transparency != 0 (not blending)
            if (*sprite_it & 0xFF000000) *ptr_toScr = *sprite_it;
            ++sprite_it;
            ++ptr_toScr;
        }
        sprite_it += rencmp.w - wSpr;
        ptr_toScr += widthScr - wSpr;
    }*/
}

template<typename GameCTX_t>
constexpr void RenderSys_t<GameCTX_t>::drawLineBox(uint32_t* ptr_toScr, uint32_t length, uint32_t displacement, uint32_t color) const noexcept
{
    while(length-- > 0) {
        *ptr_toScr = color;
        ptr_toScr += displacement;
    }
}

template<typename GameCTX_t>
constexpr void RenderSys_t<GameCTX_t>::drawAlignedLineClipped(uint32_t x1, uint32_t x2, uint32_t y, bool isYaxis, uint32_t color) const noexcept
{
    // Default values for clipping X axis
    uint32_t maxX         { widthScr    };
    uint32_t maxY         { heightScr   };
    uint32_t displacement { 1 };
    uint32_t* ptr_toScr   { nullptr };

    if (isYaxis){
        maxX = heightScr;
        maxY = widthScr;
        displacement = widthScr;
    }

    if (y >= maxY || (x1 >= maxX && x2 >= maxX )) return;

    if (x1 > maxX ) x1 = 0;
    if (x2 >= maxX) x2 = maxX-1;

    if (isYaxis) ptr_toScr = getPosition(y, x1);
    else         ptr_toScr = getPosition(x1, y);
    
    drawLineBox(ptr_toScr, x2-x1+1, displacement, color);
}

template<typename GameCTX_t>
constexpr void RenderSys_t<GameCTX_t>::drawBox(const BoundingBox<uint32_t>& box, uint32_t x, uint32_t y, uint32_t color) const noexcept
{
    // Coordinates bounding convertion to screen coordinates
    uint32_t xL { x + box.xLeft  };
    uint32_t xR { x + box.xRight };
    uint32_t yU { y + box.yUp    };
    uint32_t yD { y + box.yDown  };

    // Side sizes box
    //uint32_t widthBox  = xR - xL+1;
    //uint32_t heightBox = yD - yU+1;

    // Up line
    //drawLineBox(getPosition(xL, yU), widthBox, 1, color);
    drawAlignedLineClipped(xL, xR, yU, false, color);
    // Left line
    //drawLineBox(getPosition(xL, yU), heightBox, widthScr, color);
    drawAlignedLineClipped(yU, yD, xL, true, color);
    // Right line
    //drawLineBox(getPosition(xR, yU), heightBox, widthScr, color);
    drawAlignedLineClipped(yU, yD, xR, true, color);
    // Down line
    //drawLineBox(getPosition(xL, yD), widthBox, 1, color);
    drawAlignedLineClipped(xL, xR, yD, false, color);
}

template<typename GameCTX_t>
constexpr void RenderSys_t<GameCTX_t>::drawFillBox(const BoundingBox<uint32_t>& box, uint32_t x, uint32_t y, uint32_t color) const noexcept
{
    // Coordinates bounding convertion to screen coordinates
    uint32_t xL { x + box.xLeft  };
    uint32_t xR { x + box.xRight };
    uint32_t yU { y + box.yUp    };
    uint32_t yD { y + box.yDown  };

    // Side sizes box
    //uint32_t widthBox  = xR - xL;
    uint32_t heightBox = yD - yU+1;

    while (heightBox-- > 0){
        drawAlignedLineClipped(xL, xR, yU, false, color);
        //drawLineBox(getPosition(xL, yU), widthBox, 1, color);
        ++yU;
    }
}

template<typename GameCTX_t>
constexpr void RenderSys_t<GameCTX_t>::drawBoxTree(const BoundingBNode& treeBox, float x, float y, uint32_t color) const noexcept
{   
    // OJO!! ARM
    uint32_t xSpr { 
        (x >= 0)? static_cast<uint32_t>(std::round(x)) 
        : -static_cast<uint32_t>(std::abs(std::round(x)))                  
    };
    uint32_t ySpr { 
        (y >= 0)? static_cast<uint32_t>(std::round(y)) 
        : -static_cast<uint32_t>(std::abs(std::round(y))) 
    };

    if (treeBox.isCollided) // When a box collide, then filled box with his color.
        drawFillBox(treeBox.box, xSpr, ySpr, color);

    else drawBox(treeBox.box, xSpr, ySpr, color);

    for (const BoundingBNode& BBN : treeBox.subBoxes) drawBoxTree(BBN, x, y, color>>1);
}

template<typename GameCTX_t>
void RenderSys_t<GameCTX_t>::drawAllEntities(const GameCTX_t& contx) const noexcept
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
            drawSpriteClipped(rendercmp, *phycmp); // ON CAMERA!!

            // If debug is active, also render the bounding box.
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
constexpr void RenderSys_t<GameCTX_t>::drawAllCameras(const GameCTX_t& contx) const noexcept
{
    auto& camcmps = contx.template getCmps<CameraCmp_t>();

    for (const auto& camcmp : camcmps)
    {
        auto* phycmp = contx.template getRequiredCmp<PhysicsCmp_t>(camcmp);
        if (!phycmp) continue;

        currentCam.camcmp = &camcmp;
        currentCam.phycmp = phycmp;

        drawAllEntities(contx);
    }
}

template<typename GameCTX_t>
constexpr void RenderSys_t<GameCTX_t>::update(GameCTX_t& contx) const
{
    const uint32_t size = widthScr*heightScr;
    auto screen         = frameBuffer.get();

    std::fill(screen, screen+size, BLACK);
    drawAllCameras(contx);
    //drawAll(contx);
    ptc_update(screen);
}