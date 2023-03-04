#include <ecs/man/entityManager.cpp>
#include <game/sys/game/renderSys.hpp>
#include <game/cmp/renderCmp.hpp>
#include <game/cmp/physicsCmp.hpp>
#include <game/cmp/cameraCmp.hpp>
#include <algorithm>
#include <cmath>
#include <tuple>
#include <optional>
#include <iostream>

constexpr Box_t<float> RenderSys_t::
transformToWorldCoordinates(const Box_t<uint32_t>& box, float x, float y) const noexcept
{   
    return Box_t<float> {
            x + box.getXLeft()
        ,   y + box.getYUp()
        ,   static_cast<float>(box.w)
        ,   static_cast<float>(box.h) 
    };
}

auto RenderSys_t::transformCoordsToScreenRef(float x, float y, uint32_t width, uint32_t height) const noexcept
{
    // CURRENT IMPLEMETATION (camera added)
    // COORDINATES TRANSFORMATION:

    //   SPRT     -->     WORLD      -->      CAMERA      -->     SCREEN
    //     (0,0 + POSSPR)       (-CAMPOSWRLD)         (+CAMPOSSCR)

    std::tuple<uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t> tupla {0,0,0,0,0,0};
    auto optTuple { std::optional{tupla} };

    // VERIFY AND GET CURRENT CAMERA COMPONENTS
    if (!currentCam.camcmp || !currentCam.phycmp) { optTuple.reset(); return optTuple; }
    auto& camCmp      = *currentCam.camcmp;
    auto& phyCmpOfCam = *currentCam.phycmp;

    struct SprRefTo {   // Sprite coordinates reference ..

        Box_t<float> world  {}; // to world,
        Box_t<float> camera {}; // to camera,
        
        struct CroppedOnCam {      // pixels to crop if the sprite is not completely relative to the camera
            uint32_t left_off { 0 }, right_off { 0 };
            uint32_t up_off   { 0 }, down_off { 0 };
        } croppedOnCam {};

        struct Screen {            // and coordinates ref to screen.
            uint32_t x {0}, y {0};
        } screen{};

    } sprRef {};

    // TRANSFORM SPRITE COORDINATES INTO 'WORLD' REF (0,0 + POSSPRITE)
    sprRef.world = {       
            x
        ,   y
        ,   static_cast<float>(width+1)
        ,   static_cast<float>(height+1)
    };
    // TRANSFORM SPRITE COORDINATES INTO 'CAMERA' REF (POSSPRITEWRLD - POSCAMWRLD)
    sprRef.camera = {
            sprRef.world.x - phyCmpOfCam.x
        ,   sprRef.world.y - phyCmpOfCam.y
        ,   sprRef.world.w 
        ,   sprRef.world.h
    };

    // Get xleft, xright, yup and ydown coordinates ref on camera
    float sprCamXLeft  { sprRef.camera.getXLeft()  };
    float sprCamXRight { sprRef.camera.getXRight() };
    float sprCamYUp    { sprRef.camera.getYUp()    };
    float sprCamYDown  { sprRef.camera.getYDown()  };

    // Check the coordinates of the sprite referring to the camera is out of bounds of camera.
    if (   sprCamXLeft >= camCmp.width  || sprCamXRight < 0
        || sprCamYUp   >= camCmp.height || sprCamYDown  < 0 )
    {
        optTuple.reset(); return optTuple;
    }

    // Maybe only part of the sprite is in the camera..
    sprRef.croppedOnCam = {
            static_cast<uint32_t>(std::round((sprCamXLeft  < 0             )?  -sprCamXLeft              : 0)) // p.ejem: si left esta a -3px en x de la camara, se debe recortar 3 pixeles (-(-3)=3).
        ,   static_cast<uint32_t>(std::round((sprCamXRight >= camCmp.width )?  sprCamXRight-camCmp.width : 0))
        ,   static_cast<uint32_t>(std::round((sprCamYUp    < 0             )?  -sprCamYUp                : 0))
        ,   static_cast<uint32_t>(std::round((sprCamYDown  >= camCmp.height)?  sprCamYDown-camCmp.height : 0))
    };

    // TRANSFORM SPRITE COORDINATES INTO SCREEN REF (sprite refcam coord + cam refscreen coord) (clipped) AND NEW DIMENSIONS
    sprRef.screen = {
            camCmp.xScr + static_cast<uint32_t>(std::round(sprCamXLeft) + sprRef.croppedOnCam.left_off)  // x
        ,   camCmp.yScr + static_cast<uint32_t>(std::round(sprCamYUp)   + sprRef.croppedOnCam.up_off)    // y
    };

    /*if (currentCam.eid == 1) { // only show player coords
    std::cout<<"\ncamXscr: "<<camCmp.xScr<<" camYscr: "<<camCmp.yScr<<"\n";
    std::cout<<"camXwrld: " <<phyCmpOfCam.x<<" camYwrld: "<<phyCmpOfCam.y<<"\n";
    std::cout<<"xrefWrld: " <<x<<" yrefWrld: "<<y<<"\n";
    std::cout<<"xrefCam: "  <<sprRef.camera.xLeft<<" yrefCam: "<<sprRef.camera.yUp<<"\n";
    std::cout<<"xrefScr: "  <<sprRef.screen.x<<" yrefScr: "<<sprRef.screen.y<<"\n";
    }*/

    // CTAD: class template argument deduction:: since C++17.
    std::tuple t {
            sprRef.screen.x,               sprRef.screen.y
        ,   sprRef.croppedOnCam.left_off,  sprRef.croppedOnCam.right_off
        ,   sprRef.croppedOnCam.up_off,    sprRef.croppedOnCam.down_off
    };
    (*optTuple).swap(t);
    
    return optTuple;
}

void RenderSys_t::drawSpriteClipped(const RenderCmp_t& rencmp, const PhysicsCmp_t& phycmp) const noexcept
{   
    auto optTuple { transformCoordsToScreenRef(phycmp.x, phycmp.y, rencmp.w, rencmp.h) };
    if (!optTuple) return;

    // New coords sprite to 'screen reference' and pixels_off for sprite redimension.
    auto [xScr, yScr, l_off, r_off, u_off, d_off] = *optTuple;

    // New sprite dimensions (smaller or equal)
    uint32_t newWidth  { rencmp.w - (l_off + r_off) };
    uint32_t newHeight { rencmp.h - (u_off + d_off) };

    // RENDER SPRITE
    auto* ptr_toScr = getPosition(xScr, yScr);
    ECS::Vec_t<uint32_t>::const_iterator sprite_it  = begin(rencmp.sprite) + u_off*rencmp.w + l_off;

    while(newHeight--)
    {
        for (uint32_t i=0; i<newWidth; ++i){
            // Draw only if transparency != 0 (not blending)
            if (*sprite_it & 0xFF000000) *ptr_toScr = *sprite_it;
            ++sprite_it;
            ++ptr_toScr;
        }
        sprite_it += rencmp.w - newWidth;
        ptr_toScr += fBuff.width - newWidth;
    }
    ptr_toScr = getPosition(xScr, yScr);
    *ptr_toScr = 0xFFFFFFFF;

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
    if (xSpr > fBuff.width){                
        left_off = 0 - xSpr;               // Ejm: si xSpr esta a -3 px del limite izquierdo, entonces xSpr = 42949672963. Por lo que si se resta 0 - 42949672963 se da la vuelta y da 3.
        if (left_off >= wSpr) return;      // Nothing to draw
        wSpr -= left_off;                  // Del ejemplo, el ancho del sprite se debe recortar 3 pixeles.
        xSpr = 0;                          // Como se paso del limite izquierdo, ya no se parte pintando de una posiscion fuera del limite, sino dellimite minimo 0 del screen.
    }
    // Right clipping: recordar que la posicion x,y del sprite se encuntra en la esquina superior izquierda del sprite,
    // por lo tanto, para saber si se ah pasado del limite derecho, a la posicion x se debe sumar el ancho del sprite, y comparar si es mayo al ancho del screeen.
    else if (xSpr + wSpr >= fBuff.width){
        uint32_t right_off = xSpr + wSpr - fBuff.width; // Ejm: si x esta a 630px y w del sprite es 50px, y el ancho del screen es 640, entonces la posicion mas el ancho del sprite daria 680px, por lo que es > a 640 y se estaria pasando 40px.
        if (right_off >= wSpr) return;     // Nothing to draw
        wSpr -= right_off;                 // Del ejemplo, el ancho del sprite se debe recortar 40 pixeles, quedando con 10px de ancho para dibujar.  
        // La posicion x no cambia, ya que sigue estando en el rango del ancho de screen.                                    
    }

    // Vertical clipping rules: cuando el sprite se pasa del limite inferior o superior del screen (analogo con lo que sucede en los limites izquierdo y derecho).
    // (sucede cuando se le asigna un bounding box con alto de menor dimension e interno al sprite).

    // Up clipping
    if (ySpr > fBuff.height){
        up_off = 0 - ySpr;
        if (up_off >= hSpr) return;        // Nothing to draw
        hSpr -= up_off;
        ySpr  = 0;
    }
    // Down clipping
    else if (ySpr + hSpr >= fBuff.height){
        uint32_t down_off = ySpr + hSpr - fBuff.height;
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
        ptr_toScr += fBuff.width - wSpr;
    }*/
}

void RenderSys_t::drawLineBox(uint32_t* ptr_toScr, uint32_t length, uint32_t displacement, uint32_t color) const noexcept
{
    while(length-- > 0) {
        *ptr_toScr = color;
        ptr_toScr += displacement;
    }
}

void RenderSys_t::drawAlignedLineClipped(float xLine, float yLine, uint32_t length, bool isYaxis, uint32_t color) const noexcept
{
    // BEFORE IMPLEMENTATION!!
    // Default values for clipping X axis
    /*uint32_t maxX         { fBuff.width    };
    uint32_t maxY         { fBuff.height   };
    uint32_t displacement { 1 };
    uint32_t* ptr_toScr   { nullptr };

    if (isYaxis){
        maxX = fBuff.height;
        maxY = fBuff.width;
        displacement = fBuff.width;
    }

    if (y >= maxY || (x1 >= maxX && x2 >= maxX )) return;

    if (x1 > maxX ) x1 = 0;
    if (x2 >= maxX) x2 = maxX-1;

    if (isYaxis) ptr_toScr = getPosition(y, x1);
    else         ptr_toScr = getPosition(x1, y);*/

    // CURRENT IMPLEMENTATION WITH CAMERA
    float x             { xLine  };
    float y             { yLine  };
    uint32_t width      { length };
    uint32_t height       { 1 };
    uint32_t displacement { 1 };

    if (isYaxis){
        width        = 1;
        height       = length;
        displacement = fBuff.width;
    }

    auto optTuple { transformCoordsToScreenRef(x, y, width, height) };

    if (!optTuple) return;
    auto [xScr, yScr, l_off, r_off, u_off, d_off] = *optTuple;

    // New sprite dimensions (smaller or equal)
    uint32_t newWidth  { width  - (l_off + r_off) };
    uint32_t newHeight { height - (u_off + d_off) };
    uint32_t newLength = isYaxis? newHeight : newWidth;

    // RENDER SPRITE
    auto* ptr_toScr = getPosition(xScr, yScr);
    drawLineBox(ptr_toScr, newLength, displacement, color);
}

void RenderSys_t::drawBox(const Box_t<uint32_t>& box, float x, float y, uint32_t color) const noexcept
{
    // Coordinates bounding convertion to world coordinates
    Box_t<float> boxWrld { transformToWorldCoordinates(box, x, y) };
    float xL { boxWrld.x           };
    float xR { boxWrld.getXRight() };
    float yU { boxWrld.y           };
    float yD { boxWrld.getYDown()  };

    // Side sizes box
    uint32_t widthBox  = box.w;
    uint32_t heightBox = box.h;

    // Up line
    drawAlignedLineClipped(xL, yU, widthBox, false, color);
    // Left line
    drawAlignedLineClipped(xL, yU, heightBox, true, color);
    // Right line
    drawAlignedLineClipped(xR, yU, heightBox, true, color);
    // Down line
    drawAlignedLineClipped(xL, yD, widthBox, false, color);
}

void RenderSys_t::drawFillBox(const Box_t<uint32_t>& box, float x, float y, uint32_t color) const noexcept
{
    // X,Y coordinate bounding convertion to world coordinate.
    float xBox { x + box.x  };
    float yBox { y + box.y  };

    // Side sizes box
    uint32_t widthBox  = box.w;
    uint32_t heightBox = box.h;

    auto optTuple { transformCoordsToScreenRef(xBox, yBox, widthBox, heightBox) };

    if (!optTuple) return;
    auto [xScr, yScr, l_off, r_off, u_off, d_off] = *optTuple;

    // New box dimensions (smaller or equal)
    uint32_t newWidth  { widthBox  - (l_off + r_off) };
    uint32_t newHeight { heightBox - (u_off + d_off) };

    // RENDER FILL BOX
    auto* ptr_toScr = getPosition(xScr, yScr);

    while (newHeight-- > 0){
        drawLineBox(ptr_toScr, newWidth, 1, color);
        ptr_toScr += fBuff.width;
    }
}

void RenderSys_t::drawBoxTree(const BoundingBNode& treeBox, float x, float y, uint32_t color) const noexcept
{   
    // OJO!! ARM
    /*uint32_t xSpr { 
        (x >= 0)? static_cast<uint32_t>(std::round(x)) 
        : -static_cast<uint32_t>(std::abs(std::round(x)))                  
    };
    uint32_t ySpr { 
        (y >= 0)? static_cast<uint32_t>(std::round(y)) 
        : -static_cast<uint32_t>(std::abs(std::round(y))) 
    };*/

    if (treeBox.isCollided) // When a box collide, then filled box with his color.
        drawFillBox(treeBox.box, x, y, color);

    else drawBox(treeBox.box, x, y, color);

    for (const BoundingBNode& BBN : treeBox.subBoxes) drawBoxTree(BBN, x, y, color>>1);
}

void RenderSys_t::drawAllEntities(const ECS::EntityManager_t& contx) const noexcept
{
    // Lamda Function
    // []  Captura: pasa todo lo que esta en contexto (arma una estructura con la captura). () = parámetros
    // [=] (por copia)
    // [&] (por referencia)
    // [&algo] (especifico)
    // por defecto trata la captura como cost. Para modificar, usar 'mutable'
    /*auto getPosition = [&](uint32_t x, uint32_t y){ // <- aca 
        return fBuff + fBuff.width*y + x; // desplazarse tantas filas, luego desplazarlo en columna
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
            currentCam.eid    = phycmp->getEntityID();
            drawSpriteClipped(rendercmp, *phycmp); // ON CAMERA!!

            // If debug is active, also render the bounding box (need to implement with camera).
            if (debugDraw) 
            {
                auto* collcmp = contx.template getRequiredCmp<ColliderCmp_t>(*phycmp);
                if (!collcmp) return;
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
                ptr_toScr += fBuff.width;
            }
        }*/
        }
    );
}

void RenderSys_t::drawAllCameras(const ECS::EntityManager_t& contx) const noexcept
{
    auto& camcmps = contx.template getCmps<CameraCmp_t>();

    for (const auto& camcmp : camcmps)
    {
        auto* phycmp = contx.template getRequiredCmp<PhysicsCmp_t>(camcmp);
        if (!phycmp) continue;

        // SET CUURENT CAM
        currentCam.camcmp = &camcmp;
        currentCam.phycmp = phycmp;

        drawAllEntities(contx); // ON CAMERA!!

        // RENDER BOUNDS OF CAM
        drawLineBox(getPosition(camcmp.xScr, camcmp.yScr), camcmp.height, fBuff.width, RED);                // left
        drawLineBox(getPosition(camcmp.xScr+camcmp.width-1, camcmp.yScr), camcmp.height, fBuff.width, RED); // right
        drawLineBox(getPosition(camcmp.xScr, camcmp.yScr), camcmp.width, 1, RED);                        // up
        drawLineBox(getPosition(camcmp.xScr, camcmp.yScr+camcmp.height-1), camcmp.width, 1, RED);       // down
    }
}

void RenderSys_t::update(ECS::EntityManager_t& contx) const 
{
    const uint32_t size = fBuff.width*fBuff.height;
    auto* screen        = fBuff.get();

    std::fill(screen, screen+size, BLACK);
    drawAllCameras(contx);

    fBuff.update();
}