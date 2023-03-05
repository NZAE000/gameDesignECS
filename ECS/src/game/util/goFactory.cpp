#include <game/util/goFactory.hpp>
#include <game/cmp/inputCmp.hpp>
#include <game/cmp/healthCmp.hpp>
#include <game/cmp/cameraCmp.hpp>
#include <picoJSON.ua/picojson.hpp>
#include <fstream>

ECS::Entity_t& 
GOFactory_t::createEntity(uint32_t x, uint32_t y, const std::string_view filename) const
{ 
    auto& ent = entityMan->createEntity();

    auto& rencmp = entityMan->addCmp<RenderCmp_t>(ent);
    rencmp.loadFromPng(filename);

    auto& phycmp = entityMan->addCmp<PhysicsCmp_t>(ent);
    phycmp.x     = x;
    phycmp.y     = y;

    auto& collcmp = entityMan->addCmp<ColliderCmp_t>(ent);
    collcmp.boxRoot.box = { 0, 0, rencmp.w, rencmp.h }; // default

    [[maybe_unused]]auto& healthcmp = entityMan->addCmp<HealthCmp_t>(ent);

    return ent;
}

ECS::Entity_t& 
GOFactory_t::createPlatform(uint32_t x, uint32_t y) const
{
    auto& plataform = entityMan->createEntity();

    auto& rencmp = entityMan->addCmp<RenderCmp_t>(plataform);
    rencmp.loadFromPng("./assets/platform2.png");

    auto& phycmp = entityMan->addCmp<PhysicsCmp_t>(plataform);
    phycmp.x     = x;
    phycmp.y     = y;
    phycmp.friction = 0.85f;

    auto& collcmp = entityMan->addCmp<ColliderCmp_t>(plataform);
    collcmp.boxRoot.box = { 0, 0, rencmp.w, rencmp.h };
    collcmp.maskCollision = ColliderCmp_t::PLATFORM_LAYER;
    collcmp.property      = ColliderCmp_t::SOLID_PROP;

    return plataform;
}

ECS::Entity_t& 
GOFactory_t::createPlayer(uint32_t x, uint32_t y) const
{
    auto& principalCharac = createEntity(x, y, "./assets/deadpool2.png");
    entityMan->addCmp<InputCmp_t>(principalCharac);

    auto* collcmp = principalCharac.getCmp<ColliderCmp_t>();
    //auto* rencmp  = principalCharac.getCmp<RenderCmp_t>();

    // set boundign boxes on my principal sprite player
    //collcmp->boxRoot.box = { 0, 0, rencmp.w, rencmp.h }; // 1. bounding principal
    collcmp->boxRoot.subBoxes = {
        { { 11, 1, 37, 38 }, false, // 2. subbox
            {
                { { 16,  2, 28,  6 }, false, {} }, // 3. subbox
                { { 14,  8, 33, 22 }, false, {} }, // 3.
                { { 15, 30, 29,  8 }, false, {} }  // 3.
            } 
        },
        { { 1, 39, 47, 43 }, false, // 2.
            {
                { { 4,  40, 40, 10 }, false, {} }, // 3.
                { { 2,  50, 12, 31 }, false, {} }, // 3.
                { { 21, 50, 25, 31 }, false, {} }  // 3.
            } 
        }, 
        { { 4, 82, 44, 30 }, false, // 2.
            {
                //{ {  9, 24,  83,  100 }, false, {} }, // 3.
                //{ { 31, 44,  83, 104 }, false, {} }, // 3.
                //{ {  5, 17, 100, 110 }, false, {} }, // 3.
                //{ { 33, 46, 104, 110 }, false, {} }  // 3.
            }
        }
    };

    collcmp->maskCollision = ColliderCmp_t::FULL_LAYER ^ ColliderCmp_t::BOUNDARY_LAYER; // Colisiona con todo por defecto (evitando la colision con los limites del screen). 
    collcmp->property      = ColliderCmp_t::PLAYER_PROP;

    auto* healthcmp   = principalCharac.getCmp<HealthCmp_t>();
    healthcmp->health = 100; // set to 100 health for my player

    auto* phycmp = principalCharac.getCmp<PhysicsCmp_t>();
    phycmp->g = PhysicsCmp_t::GRAVITY; // set gravity for my player

    return principalCharac;
}

ECS::Entity_t& 
GOFactory_t::createBlade(uint32_t x, uint32_t y) const
{
    auto& blade   = createEntity(x, y, "./assets/blade.png");

    auto* phycmp = blade.getCmp<PhysicsCmp_t>();
    phycmp->vx = 2; 

    auto* collcmp = blade.getCmp<ColliderCmp_t>();
    auto* rencmp  = blade.getCmp<RenderCmp_t>();

    collcmp->boxRoot.box = { 5 ,5, rencmp->w-10, rencmp->h-10 };
    collcmp->maskCollision = ColliderCmp_t::BLADE_LAYER | ColliderCmp_t::BOUNDARY_LAYER; // Mascara compatible entre blades y limites de screen.
    collcmp->property      = ColliderCmp_t::DAMAGE_PROP;

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