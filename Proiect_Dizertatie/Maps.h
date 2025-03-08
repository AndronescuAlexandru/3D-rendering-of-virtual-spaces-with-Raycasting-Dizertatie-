#pragma once
#ifndef MAPS_H
#define MAPS_H

#include <SFML/Audio.hpp>
#include "Textures.h"
#include <fstream>

/*  map legend 

Levels 0,1,2:

# - Yellow wall 
^ - Yellow wall with red exit door
* - Dark Yellow Wall
% - concrete wall type 1
@ - concrete wall type 2
D - concrete wall type 1 with door
S - concrete wall type 2 with door
$ - Concrete wall type 1 with lamp
A - dirty concrete wall with door
W - dirty concrete wall
& - dirty concrete wall with pipes
. - empty space with yellow carpet texture ceiling and floor
; - empty space with concrete texture ceiling and floor

*/

extern unsigned int master_sound_volume;

class CurrentLevel
{
public:
    int ID; // saves the current level number where the player will be
    int ID_NextLevel;
    int MAP_WIDTH;
    int MAP_HEIGHT;
    float maxWallHeight;
    char* map;
    char* ceilingMap;
    float* heightMap;
    int machineVolume;
    int defaultWallShading;

    // colors used for floor and ceiling tiles in the current level
    sf::Color color;
    sf::Color color1;
    sf::Color color2;
    sf::Color floorColor;

    // sounds used in the current level
    sf::SoundBuffer soundBuffer;
    sf::SoundBuffer soundBuffer2;
    sf::SoundBuffer entitySoundBuffer;
    sf::SoundBuffer levelEntranceSoundBuffer;
    sf::SoundBuffer footstepsBuffer;
    sf::SoundBuffer machineSFXBuffer;

    sf::Sound AmbientSFX;
    sf::Sound AmbientSFX2;
    sf::Sound EntitySFX;
    sf::Sound levelEntranceSound;
    sf::Sound footsteps;
    sf::Sound machineSFX;

    sf::Texture Textures; // map textures

    struct SpriteObject {
        sf::Vector2f position;  // X, Y on the map
        sf::Vector2f size = sf::Vector2f(0.5f, 0.5f); // Collision size
        int textureID;          // Which sprite texture to use
        bool isLightSource = true;
        bool isActive = false; // For lamps or interactive sprites
        bool isSolid = false;  // For furniture objects
        int lightRadius = 5;  // Tiles to affect with light when active
        float distance;         // Distance from the player (for sorting)
    };

    std::vector<SpriteObject> spriteObjects; // SpriteObject container

    CurrentLevel();
    void loadMapFile(const char* levelAdress);
    void loadLevel(sf::RenderWindow& window, sf::RenderStates& state, bool defaultPlayerStartingPos);
    void loadHeightMapFile(const char* levelHeightMapAdress);
    void loadCeilingMapFile(const char* levelCeilingMapAdress);
    void loadSprites();
    ~CurrentLevel();

};

// level 0 map data

struct Level_0
{
    // map size
    const int MAP_WIDTH = 16;
    const int MAP_HEIGHT = 16;
    const float maxWallHeight = 2;
    const float defaultStartingPosX = 1.5;
    const float defaultStartingPosY = 1.5;

    const char* mapFileAdress = "Data/Maps/Level_0.map";
    const char* ceilingTilesMapFileAdress = "Data/Maps/Level_0_ceiling_Tiles.map";
    const char* textureAdress = "Data/Textures/level_1_textures.png";
    const char* ambientSFXAdress = "Data/Audio/Level0LightAmbience.mp3";
    const char* footstepsSFXAdress = "Data/Audio/CarpetFootsteps.mp3";

};

// level 1 map data

struct Level_1
{
    // map size
    const short MAP_WIDTH = 64;
    const short MAP_HEIGHT = 64;
    const float maxWallHeight = 3;

    const char* mapFileAdress = "Data/Maps/Level_5.map";
    const char* ceilingTilesMapFileAdress = "Data/Maps/Level_5_ceiling_Tiles.map";
    const char* heightMapFileAdress = "Data/Maps/Level_5_Height.map";
    const char* textureAdress = "Data/Textures/level_5_textures.png";
    const char* ambientSFXAdress = "Data/Audio/Level0LightAmbience.mp3";
    const char* footstepsSFXAdress = "Data/Audio/CarpetFootsteps.mp3";
    const char* entitySFXAdress = "Data/Audio/EntitySFX.mp3";
    const char* levelEntranceSFXAdress = "Data/Audio/FallingSFX.mp3";

};

struct Level_Test
{
    // map size
    const short MAP_WIDTH = 64;
    const short MAP_HEIGHT = 64;
    const float maxWallHeight = 3;

    const char* mapFileAdress = "Data/Maps/Level_Test.map";
    const char* textureAdress = "Data/Textures/level_4_textures.png";
    const char* ambientSFXAdress = "Data/Audio/Level0LightAmbience.mp3";
    const char* footstepsSFXAdress = "Data/Audio/CarpetFootsteps.mp3";
    const char* entitySFXAdress = "Data/Audio/EntitySFX.mp3";
    const char* levelEntranceSFXAdress = "Data/Audio/FallingSFX.mp3";

};

#endif
