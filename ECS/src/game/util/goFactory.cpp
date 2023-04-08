#include <game/util/goFactory.hpp>
#include <game/cmp/inputCmp.hpp>
#include <game/cmp/healthCmp.hpp>
#include <game/cmp/cameraCmp.hpp>
#include <picoJSON.ua/picojson.hpp>
#include <fstream>
//#include <iostream>

ECS::Entity_t& 
GOFactory_t::createEntity(uint32_t x, uint32_t y) const
{ 
    auto& ent = entityMan->createEntity();

    [[maybe_unused]]auto& rencmp = entityMan->addCmp<RenderCmp_t>(ent);

    auto& phycmp = entityMan->addCmp<PhysicsCmp_t>(ent);
    phycmp.x     = x;
    phycmp.y     = y;

    [[maybe_unused]]auto& collcmp   = entityMan->addCmp<ColliderCmp_t>(ent);
    [[maybe_unused]]auto& healthcmp = entityMan->addCmp<HealthCmp_t>(ent);

    return ent;
}

ECS::Entity_t& 
GOFactory_t::createPlatform(uint32_t x, uint32_t y) const
{
    auto& plataform = entityMan->createEntity();

    Appearance_t* platformAppear = AnimMan.getAppearance(CHARAC_t::PLATFORM, ACTION_t::DEFAULT);
    if (!platformAppear) throw std::runtime_error("Platform appearance not found");

    auto& rencmp  = entityMan->addCmp<RenderCmp_t>(plataform);
    rencmp.sprite = platformAppear->sprite.data();
    rencmp.w      = platformAppear->w;
    rencmp.h      = platformAppear->h;

    auto& collcmp         = entityMan->addCmp<ColliderCmp_t>(plataform);
    collcmp.boxRoot       = platformAppear->boxRoot; 
    collcmp.maskCollision = ColliderCmp_t::PLATFORM_LAYER;
    collcmp.property      = ColliderCmp_t::SOLID_PROP;

    auto& phycmp = entityMan->addCmp<PhysicsCmp_t>(plataform);
    phycmp.x     = x;
    phycmp.y     = y;
    phycmp.friction = 0.85f;

    return plataform;
}

ECS::Entity_t& 
GOFactory_t::createPlayer(uint32_t x, uint32_t y) const
{
    auto& principalCharac = createEntity(x, y);

    Appearance_t* playerAppear = AnimMan.getAppearance(CHARAC_t::PLAYER, ACTION_t::DEFAULT);
    if (!playerAppear) throw std::runtime_error("Player appearance not found");

    auto* rencmp   = principalCharac.getCmp<RenderCmp_t>();
    rencmp->sprite = playerAppear->sprite.data();
    rencmp->w      = playerAppear->w;
    rencmp->h      = playerAppear->h;

    auto* collcmp = principalCharac.getCmp<ColliderCmp_t>();
    collcmp->boxRoot       = playerAppear->boxRoot;
    collcmp->maskCollision = ColliderCmp_t::FULL_LAYER ^ ColliderCmp_t::BOUNDARY_LAYER; // Colisiona con todo por defecto (evitando la colision con los limites del screen). 
    collcmp->property      = ColliderCmp_t::PLAYER_PROP;

    auto* phycmp = principalCharac.getCmp<PhysicsCmp_t>();
    phycmp->g = PhysicsCmp_t::GRAVITY; // set gravity for my player

    auto& inpcmp = entityMan->addCmp<InputCmp_t>(principalCharac);

    // Set key-action to player
    inpcmp.addAction(XK_Left, [&](PhysicsCmp_t& phycmp) {
        phycmp.ax = -phycmp.STD_AX;
    });
    inpcmp.addAction(XK_Right, [&](PhysicsCmp_t& phycmp) {
        phycmp.ax = phycmp.STD_AX;
    });
    inpcmp.addAction(XK_Up, [&](PhysicsCmp_t& phycmp) {
        if (phycmp.isJumpEnabled()) {
            phycmp.startJumpPhase();
            //std::cout<<"JUMP\n";
        }
    });

    auto* healthcmp   = principalCharac.getCmp<HealthCmp_t>();
    healthcmp->health = 100; // set to 100 health for my player

    return principalCharac;
}

ECS::Entity_t& 
GOFactory_t::createBlade(uint32_t x, uint32_t y) const
{
    auto& blade = createEntity(x, y);

    Appearance_t* bladeAppear = AnimMan.getAppearance(CHARAC_t::BLADE, ACTION_t::DEFAULT);
    if (!bladeAppear) throw std::runtime_error("Blade appearance not found");

    auto* rencmp  = blade.getCmp<RenderCmp_t>();
    rencmp->sprite = bladeAppear->sprite.data();
    rencmp->w      = bladeAppear->w;
    rencmp->h      = bladeAppear->h;

    auto* collcmp = blade.getCmp<ColliderCmp_t>();
    collcmp->boxRoot       = bladeAppear->boxRoot;
    collcmp->maskCollision = ColliderCmp_t::BLADE_LAYER | ColliderCmp_t::BOUNDARY_LAYER; // Mascara compatible entre blades y limites de screen.
    collcmp->property      = ColliderCmp_t::DAMAGE_PROP;

    auto* phycmp = blade.getCmp<PhysicsCmp_t>();
    phycmp->vx = 250; // per seconds

    auto* healthcmp = blade.getCmp<HealthCmp_t>();
    healthcmp->selfDmgOnInfliction = 1;
    healthcmp->inflictedDmg        = 1;

    return blade;
}

ECS::Entity_t& 
GOFactory_t::createCamera(uint32_t x, uint32_t y, uint32_t w, uint32_t h, ECS::EntityID_t eid) const
{
    auto& cam = entityMan->createEntity();

    auto& camcmp = entityMan->addCmp<CameraCmp_t>(cam);
    camcmp.xScr  = x; camcmp.yScr   = y;
    camcmp.width = w; camcmp.height = h;
    camcmp.followEntID = eid;

    [[maybe_unused]] auto& phycmp = entityMan->addCmp<PhysicsCmp_t>(cam);

    return cam;
}

void GOFactory_t::createLevel1() const
{
    // --------- Entities ------------
    
    /*constexpr std::array map = {
            0b00000000
        ,   0b00000000
        ,   0b00000000
        ,   0b01100001
        ,   0b00000001
        ,   0b00000011
        ,   0b00000111
        ,   0b00001111
        ,   0b11101111
        ,   0b10000001
        ,   0b10000001
        ,   0b10000001
        ,   0b10000001
        ,   0b10111011
        ,   0b10010011
        ,   0b10010011
        ,   0b10010011
        ,   0b10010001
        ,   0b11011111
        ,   0b10000001
        ,   0b10000001
        ,   0b10000001
        ,   0b10000001
        ,   0b10000001
        ,   0b11111111
    };

    uint32_t y {0};
    for (auto row: map){
        for (uint32_t x=0; x<8*100; x+=100){
            if (row & 0x80) createPlatform(x, y);
            row <<= 1;
        }
        y+=41;
    }*/

    // Entidad spawner que genera n entidades cada cierto lapso de tiempo
    // Ultimo argumento es un llamable que es la accion al momento de spawnear,
    // que en este caso es crear un nuevo blade.
    [[maybe_unused]]auto& spawner = createSpawner(605, 1, [&](const SpawnCmp_t& spcp)
    {
        auto* phycmp = entityMan->getRequiredCmp<PhysicsCmp_t>(spcp);
        if (!phycmp) return;

        auto& ent  = createBlade(phycmp->x,phycmp->y);
        phycmp     = ent.getCmp<PhysicsCmp_t>(); // los blades generados solo se desplazan en la ordenada
        phycmp->vx *= -1;
    });

    //createCamera(400, 0 , 240, 360, spawner.getEntityID());
}

void GOFactory_t::loadLevelFromJSON(const std::string_view jsonpath) const
{
    namespace PJ = picojson;

    std::ifstream file(jsonpath.data()); // Open input stream json file

    if (file.fail()) throw std::runtime_error("Can't open json file");

    PJ::value json; // json value ( all {...} )
    file>>json;     // Read filejson into json value

    // Check if json value not obtained
    std::string err { PJ::get_last_error() };
    if (!err.empty()) throw std::runtime_error(err);

    // Process and get the content of the json (types into json: obj, array, float, bool)
    const auto& root  { json.get<PJ::object>() }; // get principal object {}

    // Get keys from root
    const auto& width  { root.at("width").get<double>()  };
    const auto& height { root.at("height").get<double>() };
    const auto& map    { root.at("map").get<PJ::array>() };

    if (width*height != map.size()) throw std::runtime_error("Map size error"); 


    // CAMBIAR
    /*auto& player = createPlayer(0, 0);
    createCamera(0, 0, 640, 360, player.getEntityID());

    uint32_t x {0}, y{0};
    for (const auto& elem : map)
    {   
        auto active { static_cast<bool>(elem.get<double>()) }; 
        if (active) createPlatform(x*100, y*41);
        if (++x == width) { x=0; ++y; };
    }

    createLevel1();*/
}


void GOFactory_t::
createBinLevelFromJSON(const std::string_view jsonpath, const std::string_view binpath) const
{
    namespace PJ = picojson;

    std::ifstream jsonfile(jsonpath.data()); // Open input stream json file
    if (jsonfile.fail()) throw std::runtime_error("Can't open json file");

    std::ofstream binfile(binpath.data(), std::ostream::binary | std::ostream::trunc); // Open input stream json file
    if (binfile.fail()) throw std::runtime_error("Can't open bin file");

    PJ::value json;     // json value ( all {...} )
    jsonfile>>json;     // Read filejson into json value

    // Check if json value not obtained
    std::string err { PJ::get_last_error() };
    if (!err.empty()) throw std::runtime_error(err);

    // Process and get the content of the json (types into json: obj, array, float, bool)
    const auto& root  { json.get<PJ::object>() }; // get principal object {}

    // Get keys from root
    const auto& width  { static_cast<uint32_t>(root.at("width").get<double>() ) };
    const auto& height { static_cast<uint32_t>(root.at("height").get<double>()) };
    const auto& map    { root.at("map").get<PJ::array>() };

    if (width*height != map.size()) throw std::runtime_error("Map size error");

    binfile.write(reinterpret_cast<const char*>(&width), sizeof width);
    binfile.write(reinterpret_cast<const char*>(&height), sizeof height);

    for (const auto& elem : map)
    {   
        uint8_t tile { static_cast<uint8_t>(elem.get<double>()) }; // 0 or 1
        binfile.write(reinterpret_cast<const char*>(&tile), sizeof tile);
    }
}

void GOFactory_t::loadLevelFromBin(const std::string_view path) const
{
    std::ifstream binFile(path.data(), std::ifstream::binary);
    if (binFile.fail()) throw std::runtime_error("Can't open bin file");
    
    // GET TOTAL SIZE OF BIN CONTEN
    std::size_t sizeFile = [&]() {

        binFile.seekg(0, binFile.end);
        auto size = binFile.tellg();
        binFile.seekg(0, binFile.beg);

        return size;

    }(); // Immediately invoke lambda

    if (sizeFile < 8) throw std::runtime_error("Bin file level corrupt. Small size");

    // INIT TOTAL CAPACITY FOR STORE LEVEL
    ECS::Vec_t<char> levelData(sizeFile);
    char* ptrLevel = &levelData[0];

    binFile.read(ptrLevel, sizeFile); // Read byte to byte from bin memory into vector.

    // GET KEYS: WIDHT, HEIGHT, MAP ..

    // first widht and height..
    uint32_t width  {0}, height {0};

    std::memcpy(&width,  ptrLevel + 0, sizeof width);
    std::memcpy(&height, ptrLevel + 4, sizeof height);
    if (width*height != (sizeFile - 8))  throw std::runtime_error("Bad map size form bin file leve");

    // then the map
    ptrLevel += 8; // 8x8 = 64bits(8 bytes (4 bytes and 4 bytes)) more for pointer.

    // CAMBIAR
    auto& player = createPlayer(100, 0);
    createCamera(0, 0, 639, 359, player.getEntityID());
    
    uint32_t x{0}, y{0};
    do
    {   
       if (*ptrLevel) createPlatform(x*100, y*41);
       if (++x == width) { x=0; ++y; }

    } while(++ptrLevel != (levelData.data() + sizeFile));

    /*for (uint32_t y=0; y<height; ++y){
        for (uint32_t x=0; x<width; ++x)
        {
            if (*ptrLevel==1) createPlatform(x*100, y*41);
            ptrLevel++;
        }
    }*/

    createLevel1();
}