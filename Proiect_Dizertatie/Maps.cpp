#include "Maps.h"
#include "Player.h"

class CurrentLevel currentLevel;

struct Level_0 level_0;
struct Level_1 level_1;
struct Level_Test level_test;

extern float wallShading;
extern float ceilingShading;
extern float floorShading;
extern int RENDER_DISTANCE;

extern Player player;
extern bool noSaveFile;
extern sf::Clock globalClock;

extern void SetBlackScreen(sf::RenderWindow& window, sf::Time seconds);

CurrentLevel::CurrentLevel()
{
    ID = 0;
    MAP_WIDTH = 384;
    MAP_HEIGHT = 384;
    maxWallHeight = 1;
}

CurrentLevel::~CurrentLevel()
{
    free(map);
    free(heightMap);
}

void CurrentLevel::loadMapFile(const char* levelAdress)
{
    std::ifstream levelFile(levelAdress);

    map = (char*)malloc((MAP_HEIGHT * MAP_WIDTH) * sizeof(char));

    if (map == nullptr)
    {
        printf("Could not allocate memory for map variable from Maps.h/Maps.cpp");
        return;
    }

    if (!levelFile.is_open())
    {
        printf("Could not open level data file or it doesn't exist; Exiting program");
        free(map);
        return;
    }

    for (int i = 0; i <= MAP_HEIGHT * MAP_WIDTH; i++)
    {
        levelFile >> map[i];
        if (levelFile.eof())
            break;
    }

    levelFile.close();
}

void CurrentLevel::loadHeightMapFile(const char* levelHeightMapAdress)
{
    std::ifstream heightMapFile(levelHeightMapAdress);

    heightMap = (float*)malloc((MAP_HEIGHT * MAP_WIDTH) * sizeof(float));

    if (heightMap == nullptr)
    {
        printf("Could not allocate memory for pointer level_0.map!");
        return;
    }

    if (!heightMapFile.is_open())
    {
        printf("Could not open data file Level_0.txt or file doesn't exist; Exiting program");
        free(map);
        return;
    }

    for (int i = 0; i <= MAP_HEIGHT * MAP_WIDTH; i++)
    {
        heightMapFile >> heightMap[i];
        if (heightMapFile.eof())
            break;
    }

    heightMapFile.close();
}

void CurrentLevel::loadCeilingMapFile(const char* levelCeilingMapAdress)
{
    std::ifstream ceilingMapFile(levelCeilingMapAdress);

    ceilingMap = (char*)malloc((MAP_HEIGHT * MAP_WIDTH) * sizeof(char));

    if (ceilingMap == nullptr)
    {
        printf("Could not allocate memory for pointer level_0.map!");
        return;
    }

    if (!ceilingMapFile.is_open())
    {
        printf("Could not open data file %c levelCeilingMapAdress or file doesn't exist; Exiting program", levelCeilingMapAdress);
        free(map);
        return;
    }

    for (int i = 0; i <= MAP_HEIGHT * MAP_WIDTH; i++)
    {
        ceilingMapFile >> ceilingMap[i];
        if (ceilingMapFile.eof())
            break;
    }

    ceilingMapFile.close();
}

void CurrentLevel::loadSprites()
{
    switch (ID)
    {
    case 0:
        spriteObjects.push_back({ sf::Vector2f(5.5f, 3.5f), 0 });
        spriteObjects.push_back({ sf::Vector2f(8.0f, 6.0f), 1 });
        break;
    case 1:

        break;
    case 2:

        break;
    default:
        break;
    }
}

void CurrentLevel::loadLevel(sf::RenderWindow& window, sf::RenderStates& state, bool defaultPlayerStartingPos)
{
    footsteps.setVolume(master_sound_volume);
    levelEntranceSound.setVolume(master_sound_volume);

    if (!entitySoundBuffer.loadFromFile("Data/Audio/EntitySFX.mp3"))
    {
        printf("Cannot open sound file EntitySFX.mp3!\n");
    }

    EntitySFX.setBuffer(currentLevel.entitySoundBuffer);
    // loads files specific for each level and set render state that uses the texture
    switch (ID)
    {
    case 0:
    {
        loadSprites();
        loadMapFile(level_0.mapFileAdress);
        if (!Textures.loadFromFile(level_0.textureAdress))
        {
            printf("Cannot open file %c\n", level_0.textureAdress);
        }

        if (!soundBuffer.loadFromFile(level_0.ambientSFXAdress))
        {
            printf("Cannot open sound file %c\n", level_0.ambientSFXAdress);
        }

        if (!footstepsBuffer.loadFromFile(level_0.footstepsSFXAdress))
            printf("Cannot open sound file %c\n", level_0.footstepsSFXAdress);

        footsteps.setBuffer(footstepsBuffer);

        MAP_HEIGHT = level_0.MAP_HEIGHT;
        MAP_WIDTH = level_0.MAP_WIDTH;
        maxWallHeight = level_0.maxWallHeight;

        state.texture = &Textures;

        AmbientSFX.setBuffer(soundBuffer);
        AmbientSFX.setVolume(0.5 * master_sound_volume);
        AmbientSFX.play();
        AmbientSFX.setLoop(true);

        defaultWallShading = 1.1;
        wallShading = 1.1;

        color1 = sf::Color(182, 179, 102);
        color2 = sf::Color(255, 255, 255);

        floorColor = sf::Color(178, 163, 106);

        if(noSaveFile == true || defaultPlayerStartingPos == true)
            player.setPlayerNewPos(66, 38);

        break;
    }
    case 1:
    {
        MAP_HEIGHT = level_1.MAP_HEIGHT;
        MAP_WIDTH = level_1.MAP_WIDTH;

        loadMapFile(level_1.mapFileAdress);
        loadHeightMapFile(level_1.heightMapFileAdress);
        loadCeilingMapFile(level_1.ceilingTilesMapFileAdress);

        if (!Textures.loadFromFile(level_1.textureAdress))
        {
            printf("Cannot open sound file %c\n", level_1.textureAdress);
        }

        if (!soundBuffer.loadFromFile(level_1.ambientSFXAdress))
        {
            printf("Cannot open sound file %c\n", level_1.ambientSFXAdress);
        }

        if (!footstepsBuffer.loadFromFile(level_1.footstepsSFXAdress))
            printf("Cannot open sound file %c\n", level_1.footstepsSFXAdress);

        footsteps.setBuffer(footstepsBuffer);

        maxWallHeight = level_1.maxWallHeight;

        state.texture = &Textures;
        wallShading = 1.1;
        defaultWallShading = 1.1;

        color1 = sf::Color(255, 255, 255);
        color2 = sf::Color(255, 255, 255);
        floorColor = sf::Color(255, 255, 255);
        floorColor.r /= 1.2;
        floorColor.g /= 1.2;
        floorColor.b /= 1.2;

        if (defaultPlayerStartingPos)
            player.setPlayerNewPos(4.5, 31.5);

        break;
    }
    case 2:
    {
        loadMapFile(level_test.mapFileAdress);
        if (!Textures.loadFromFile(level_test.textureAdress))
        {
            printf("Cannot open file %c\n", level_test.textureAdress);
        }

        if (!soundBuffer.loadFromFile(level_test.ambientSFXAdress))
        {
            printf("Cannot open sound file %c\n", level_test.ambientSFXAdress);
        }

        if (!footstepsBuffer.loadFromFile(level_test.footstepsSFXAdress))
            printf("Cannot open sound file %c\n", level_test.footstepsSFXAdress);

        footsteps.setBuffer(footstepsBuffer);

        MAP_HEIGHT = level_test.MAP_HEIGHT;
        MAP_WIDTH = level_test.MAP_WIDTH;
        maxWallHeight = level_test.maxWallHeight;

        state.texture = &Textures;

        AmbientSFX.setBuffer(soundBuffer);
        AmbientSFX.setVolume(0.5 * master_sound_volume);
        AmbientSFX.play();
        AmbientSFX.setLoop(true);

        defaultWallShading = 1.1;
        wallShading = 1.1;

        color1 = sf::Color(182, 179, 102);
        color2 = sf::Color(255, 255, 255);

        floorColor = sf::Color(175, 175, 175);

        if (noSaveFile == true || defaultPlayerStartingPos == true)
            player.setPlayerNewPos(2, 2);

        break;
    }
    default:

        printf("Selected level is invalid. Loading the default level instead.");

        loadMapFile(level_0.mapFileAdress);
        if (!Textures.loadFromFile(level_0.textureAdress))
        {
            printf("Cannot open file %c\n", level_0.textureAdress);
        }

        if (!soundBuffer.loadFromFile(level_0.ambientSFXAdress))
        {
            printf("Cannot open sound file %c\n", level_0.ambientSFXAdress);
        }

        if (!footstepsBuffer.loadFromFile(level_0.footstepsSFXAdress))
            printf("Cannot open sound file %c\n", level_0.footstepsSFXAdress);

        footsteps.setBuffer(footstepsBuffer);

        MAP_HEIGHT = level_0.MAP_HEIGHT;
        MAP_WIDTH = level_0.MAP_WIDTH;
        maxWallHeight = level_0.maxWallHeight;

        state.texture = &Textures;

        AmbientSFX.setBuffer(soundBuffer);
        AmbientSFX.setVolume(0.5 * master_sound_volume);
        AmbientSFX.play();
        AmbientSFX.setLoop(true);

        defaultWallShading = 1.1;
        wallShading = 1.1;

        color1 = sf::Color(182, 179, 102);
        color2 = sf::Color(255, 255, 255);

        floorColor = sf::Color(178, 163, 106);

        if (noSaveFile == true || defaultPlayerStartingPos == true)
            player.setPlayerNewPos(66, 38);

        break;
    }
}