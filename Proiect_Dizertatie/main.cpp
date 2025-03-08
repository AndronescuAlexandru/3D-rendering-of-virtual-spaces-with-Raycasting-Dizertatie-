// Version : 0.0.1

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <random>
#include <string.h>
#include <chrono>
#include <algorithm>
#include <SFML/System/Time.hpp>
#include <Windows.h>
#include <fstream>

#include "Maps.h"
#include "Player.h"
#include "Textures.h"
#include "Constants.h"
#include "Menus.h"
#include "FileOperations.h"

extern Player player;
extern CurrentLevel currentLevel;

sf::SoundBuffer soundEffectsBuffer;
sf::Sound soundEffect;
sf::Clock globalClock; // clock that is used globally for multiple calculations
sf::Clock gameClock; // global game clock that starts counting when the game is started
sf::Clock eventsGlobalClock; // events global clock that starts counting the seconds since the last event occured

sf::Vector2i prevMousePosition;
const float mouseSensitivity = 0.002f; // Adjust for faster or slower rotation
const float verticalSensitivity = 0.001f; // Adjust as needed
float verticalLookOffset = 0.0f;
const float verticalLookSensitivity = 0.2f; // Adjust for faster/slower tilt
const float maxVerticalLookUp = 200.0f; // Maximum pixels to tilt when looking up
const float maxVerticalLookDown = 100.0f; // Maximum pixels to tilt when looking down

bool keyboardPressed = false;
bool levelChanged = false;
bool playerIsMoving = false;
bool footstepsPlaying = false;
bool debugMode = true;
bool noSaveFile;
bool hasFocus = true;
bool randomEvents = false;

float wallShading = 1.5;
float ceilingShading = 1.5;
float floorShading = 1.5;
int gameState = 0;
int RENDER_DISTANCE = 8;
int RENDER_DISTANCE_COPY = RENDER_DISTANCE; // the value of this copy of the variable RENDER_DISTANCE is used to revert back to the saved render distance settings
int SPRITE_RENDER_DISTANCE = 16;
// after an event that changed the lighting occured
unsigned int master_sound_volume = 100;
float menuSpriteScaleX = 1.25, menuSpriteScaleY = 1;

// sets default screen resolution

int screenWidth = 800;
int screenHeight = 600;

float cameraHeight = 0.66; // height of player camera(1.0 is ceiling, 0.0 is floor)

const int texture_size = 512; // size(width and height) of texture that will hold all wall textures
const int texture_wall_size = 128; // size(width and height) of each wall type in the full texture

const float fps_refresh_time = 0.1; // time between FPS text refresh. FPS is smoothed out over this time

char GetTile(int x, int y, short level)
{
    return currentLevel.map[y * currentLevel.MAP_WIDTH + x];
}

float GetHeight(int x, int y, short level)
{
    if (level == 1)
        return currentLevel.heightMap[y * currentLevel.MAP_WIDTH + x];

    return currentLevel.maxWallHeight;
}

void UpdateFPS(sf::Text& fpsText, float dt, int64_t frame_time_micro)
{
    static float dt_counter = 0.0; // delta time for multiple frames, for calculating FPS smoothly
    static int frame_counter = 0; // counts frames for FPS calculation
    char frameInfoString[sizeof("FPS: *****.*")];

    // Update FPS, smoothed over time
    if (dt_counter >= fps_refresh_time)
    {
        float fps = (float)frame_counter / dt_counter;
        frame_time_micro /= frame_counter;
        snprintf(frameInfoString, sizeof(frameInfoString), "FPS: %3.1f", fps);
        fpsText.setString(frameInfoString);
        dt_counter = 0.0;
        frame_counter = 0;
        frame_time_micro = 0;
    }
    dt_counter += dt;
    frame_counter++;
}

float get_degrees(const float i_degrees)
{
    return static_cast<float>(fmod(360 + fmod(i_degrees, 360), 360));
}

float deg_to_rad(const float i_degrees)
{
    return PI * get_degrees(i_degrees) / 180;
}

void PlaySFX(short type, sf::Sound& soundEffect, int volume)
{
    if (volume != -1)
    {
        soundEffect.setVolume(volume);

        switch (type)
        {
        case 0:
        {
            if (!soundEffectsBuffer.loadFromFile("Data/Audio/MetalDoorOpeningClosing.mp3"))
                std::cout << "Could not open sound file MetalDoorOpeningClosing.mp3!\n";

            soundEffect.setBuffer(soundEffectsBuffer);
            soundEffect.play();

            break;
        }
        case 1:
        {
            if (!soundEffectsBuffer.loadFromFile("Data/Audio/FallingSFX.mp3"))
                std::cout << "Could not open sound file FallingSFX.mp3!\n";

            soundEffect.setBuffer(soundEffectsBuffer);
            soundEffect.play();
            break;
        }
        case 2:
        {
            if (!soundEffectsBuffer.loadFromFile("Data/Audio/ElevatorDoorOpening.mp3"))
                std::cout << "Could not open sound file ElevatorDoorOpening.mp3!\n";

            soundEffect.setBuffer(soundEffectsBuffer);
            soundEffect.play();
            break;
        }
        case 3:
        {
            if (!soundEffectsBuffer.loadFromFile("Data/Audio/ElevatorDoorClosing.mp3"))
                std::cout << "Could not open sound file ElevatorDoorClosing.mp3!\n";

            soundEffect.setBuffer(soundEffectsBuffer);
            soundEffect.play();
            break;
        }
        case 4:
        {
            if (!soundEffectsBuffer.loadFromFile("Data/Audio/DistortedScream.mp3"))
                std::cout << "Could not open sound file WomanWilhelmScream.mp3!\n";

            soundEffect.setBuffer(soundEffectsBuffer);
            soundEffect.play();
            break;
        }
        case 5:
        {
            if (!soundEffectsBuffer.loadFromFile("Data/Audio/WalkingSFX.mp3"))
                std::cout << "Could not open sound file WalkingSFX.mp3!\n";

            soundEffect.setBuffer(soundEffectsBuffer);
            soundEffect.play();
            break;
        }
        case 6:
        {
            if (!soundEffectsBuffer.loadFromFile("Data/Audio/EntitySFX.mp3"))
            {
                printf("Cannot open sound file EntitySFX.mp3!\n");
            }
            soundEffect.setBuffer(soundEffectsBuffer);
            soundEffect.play();
            break;
        }
        default:
            break;
        }
    }
}

void KeyboardInput(bool hasFocus, Player& player, sf::Vector2f size, float dt) // handles keyboard input
{
    bool coordinatesRecentlyChanged = false;
    bool crouching = false;
    if (hasFocus)
    {
        using kb = sf::Keyboard;

        // moving forward or backwards (1.0 or -1.0)
        float moveForward = 0.0;
        bool elevatorCalled = false;

        if (gameState == IN_GAME)
        {
            // player controls
            // 
            // rotating rightwards or leftwards(1.0 or -1.0)
            float rotateDirection = 0.0;

            if (kb::isKeyPressed(kb::W))
            {
                moveForward = 1.0;
            }
            else
                if (kb::isKeyPressed(kb::S))
                {
                    moveForward = -1.0;
                }
                else
                {
                    footstepsPlaying = false;
                    currentLevel.footsteps.stop();
                }

            if (kb::isKeyPressed(kb::LShift))
            {
                player.playerMoveSpeed = 3.5;
                currentLevel.footsteps.setPitch(1.4);
            }
            else if (player.playerMoveSpeed >= 3.5)
            {
                player.playerMoveSpeed = 2;
                currentLevel.footsteps.setPitch(0.9);
            }

            if (kb::isKeyPressed(kb::Space) && globalClock.getElapsedTime().asMilliseconds() >= 500)
            {
                cameraHeight = 0.80;
                globalClock.restart();
            }

            if (kb::isKeyPressed(kb::LControl))
            {
                cameraHeight = 0.45;
                player.playerMoveSpeed = 1;
                currentLevel.footsteps.setPitch(0.65);
            }
            else if (cameraHeight <= 0.1)
            {
                cameraHeight = 0.66;
                player.playerMoveSpeed = 2;
                currentLevel.footsteps.setPitch(0.9);
            }

            if (kb::isKeyPressed(kb::A))
            {
                rotateDirection = -1.0;
            }
            else if (kb::isKeyPressed(kb::D))
            {
                rotateDirection = 1.0;
            }

            // handle rotation
            if (rotateDirection != 0.0)
            {
                float rotation = player.playerRotateSpeed * rotateDirection * dt;
                player.direction = player.Rotate(player.direction, rotation);
                player.plane = player.Rotate(player.plane, rotation);
            }

            // debug controls
            if (kb::isKeyPressed(kb::Equal))
                debugMode = true;

            if (kb::isKeyPressed(kb::Hyphen))
                debugMode = false;

            if (debugMode == true)
            {
                if (kb::isKeyPressed(kb::Left)) // shortens the render distance and darkens the wall shading
                {
                    wallShading = 4;
                    ceilingShading = 4;
                    floorShading = 4;
                    RENDER_DISTANCE = 4;
                }

                if (kb::isKeyPressed(kb::Right))
                {
                    wallShading = 1.5;
                    RENDER_DISTANCE = 16;
                }

                if (kb::isKeyPressed(kb::Q))
                {
                    std::cout << "Enter player's new X position\n";
                    std::cin >> player.position.x;
                    std::cout << "Enter player's new Y position\n";
                    std::cin >> player.position.y;
                }

                if (kb::isKeyPressed(kb::Up) && cameraHeight < 0.95)
                {
                    cameraHeight += 0.1 * dt;
                    keyboardPressed = true;
                }
                else
                {
                    keyboardPressed = false;
                }

                if (kb::isKeyPressed(kb::Down) && cameraHeight > 0.1)
                    cameraHeight -= 0.1 * dt;

                if (kb::isKeyPressed(kb::L))
                {
                    std::cout << "Enter level ID to load \n";
                    std::cin >> currentLevel.ID_NextLevel;

                    levelChanged = true;
                }

            }

            if (kb::isKeyPressed(kb::E))
            {
                if (currentLevel.ID == 0 && (int)player.position.x == 2 && (int)player.position.y == 1)
                {
                    currentLevel.ID_NextLevel = 6;
                    PlaySFX(0, soundEffect, 100);

                    levelChanged = true;
                }

                if (currentLevel.ID == 0 && (((int)player.position.x == 1 && (int)player.position.y == 99)
                    || ((int)player.position.x == 126 && (int)player.position.y == 17)))
                {
                    currentLevel.ID_NextLevel = 1;
                    //playSFX(1, soundEffect);
                    levelChanged = true;
                }

                if (currentLevel.ID == 1 && (int)player.position.x == 1 && (int)player.position.y == 2)
                {
                    PlaySFX(0, soundEffect, 100);
                    player.setPlayerNewPos(5, 2.5);
                    coordinatesRecentlyChanged = true;
                }

                if (currentLevel.ID == 1 && (int)player.position.x == 4 && (int)player.position.y == 2)
                {
                    PlaySFX(0, soundEffect, 100);
                    player.setPlayerNewPos(1.5, 3);
                    coordinatesRecentlyChanged = true;
                }

                for (auto& sprite : currentLevel.spriteObjects) 
                {
                    float dx = sprite.position.x - player.position.x;
                    float dy = sprite.position.y - player.position.y;
                    float distance = sqrt(dx * dx + dy * dy);

                    if (distance < 1.5f)
                    { // Within interaction range
                        sprite.isActive = !sprite.isActive; // Toggle state
                        break; // Only interact with the closest sprite
                    }
                }
            }

            if (moveForward != 0.0f)
            {
                if (footstepsPlaying == false)
                {
                    currentLevel.footsteps.play();
                    currentLevel.footsteps.setLoop(true);
                    footstepsPlaying = true;
                }

                sf::Vector2f moveVec = player.direction * player.playerMoveSpeed * moveForward * dt;
                if (player.CanMove(sf::Vector2f(player.position.x + moveVec.x, player.position.y), size)) {
                    player.position.x += moveVec.x;
                }
                if (player.CanMove(sf::Vector2f(player.position.x, player.position.y + moveVec.y), size)) {
                    player.position.y += moveVec.y;
                }
            }

            // game controls
            if (kb::isKeyPressed(kb::Escape))
            {
                gameState = PAUSE;

            }

            coordinatesRecentlyChanged = false;
        }

    }
}

float square_root(float x) // function that calculates the square root faster (uses the algorithm from DOOM and Wolfenstein games)
{
    int i = *(int*)&x; // get bits for floating value
    i = (i >> 1); // gives initial guess y0
    x = *(float*)&i; // convert bits back to float

    return x;
}

void get_textures(int& wallTextureNum, char tile) // retrieves the corresponding tile texture from level texture file
{
    switch (currentLevel.ID)
    {
    case 0:
        wallTextureNum = (int)level0_wallTypes.find(tile)->second;
        break;
    case 1:
        wallTextureNum = (int)level1_wallTypes.find(tile)->second;
        break;
    default:
        wallTextureNum = (int)level0_wallTypes.find(tile)->second;
        break;
    }
}

char get_ceiling_tile(int x, int y)
{

    return currentLevel.ceilingMap[y * currentLevel.MAP_WIDTH + x];
}

void Raycasting(sf::RenderWindow& window, sf::RenderStates state, sf::VertexArray& lines)
{
    window.clear();
    lines.clear();
    lines.resize(0);

    std::vector<float> wallDistances(screenWidth, RENDER_DISTANCE);
    sf::Vector2f spritePos;

    float invDet;

    float transformX;
    float transformY;

    int spriteScreenX;
    int spriteHeight;
    int spriteWidth;
    const int maxSpriteSize = screenHeight;
    int drawStartY;
    int drawEndY;
    int drawStartX;
    int drawEndX;
    int textureXStart;
    int textureX;
    const size_t maxVerticesPerFrame = 1000000;

    // loop through vertical screen lines, draw a line of wall for each
    for (int x = 0; x < screenWidth; x++)
    {
        char tile = '.'; // tile type that got hit
        bool horizontal; // did we hit a horizontal side? Otherwise it's vertical

        float perpWallDist = 0.0; // wall distance, projected on camera direction
        float wall_x;
        float heightTile = 1;
        int groundHeight = 1;
        int wallHeight; // height of wall to draw on the screen at each distance
        int shading = 1.5;
        int lightSourcePosX, lightSourcePosY;
        int distance;

        float ceilingPixel = 0; // position of ceiling pixel on the screen
        float groundPixel = screenHeight; // position of ground pixel on the screen
        int wallTextureNum = 0; // number of texture from the texture file
        int ceilingTextureNum = 0;

        sf::Vector2i step; // what direction to step in (+1 or -1 for each dimension)
        sf::Vector2f sideDist; // distance from current position to next gridline, for x and y separately

        // ray to emit
        float cameraX = 2 * x / (float)screenWidth - 1.0; // x in camera space (between -1 and +1)
        sf::Vector2f rayPos = player.position;
        sf::Vector2f rayDir = player.direction + player.plane * cameraX;
        sf::Vector2i mapPos(rayPos); // which box of the map we're in

        // calculate distance traversed between each grid line for x and y based on direction
        sf::Vector2f deltaDist(
            sqrt(1.0 + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x)),
            sqrt(1.0 + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y))
        );

        // calculate step and initial sideDist
        if (rayDir.x < 0.0) {
            step.x = -1;
            sideDist.x = (rayPos.x - mapPos.x) * deltaDist.x;
        }
        else {
            step.x = 1;
            sideDist.x = (mapPos.x + 1.0f - rayPos.x) * deltaDist.x;
        }
        if (rayDir.y < 0.0f) {
            step.y = -1;
            sideDist.y = (rayPos.y - mapPos.y) * deltaDist.y;
        }
        else {
            step.y = 1;
            sideDist.y = (mapPos.y + 1.0f - rayPos.y) * deltaDist.y;
        }

        // cast the ray until we hit a wall, meanwhile draw floors
        while (tile == '.' || tile == ';' || tile == ',')
        {
            currentLevel.color = ((mapPos.x % 2 == 0 && mapPos.y % 2 == 0) || (mapPos.x % 2 == 1 && mapPos.y % 2 == 1)) ? currentLevel.color1 : currentLevel.color2;

            if (sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                mapPos.x += step.x;
                horizontal = true;
                perpWallDist = (mapPos.x - rayPos.x + (1 - step.x) / 2) / rayDir.x;
                wall_x = rayPos.y + perpWallDist * rayDir.y;
            }
            else
            {
                sideDist.y += deltaDist.y;
                mapPos.y += step.y;
                horizontal = false;
                perpWallDist = (mapPos.y - rayPos.y + (1 - step.y) / 2) / rayDir.y;
                wall_x = rayPos.x + perpWallDist * rayDir.x;
            }

            // Check if the wall is beyond the maximum rendering distance
            if (perpWallDist > RENDER_DISTANCE)
                break;

            wallDistances[x] = perpWallDist;

            tile = GetTile(mapPos.x, mapPos.y, currentLevel.ID);
            heightTile = GetHeight(mapPos.x, mapPos.y, currentLevel.ID);

            sf::Vector2i floorTextureCoords(
                wallTextureNum * texture_wall_size % texture_size,
                wallTextureNum * texture_wall_size / texture_size * texture_wall_size
            );

            wallHeight = (screenHeight / perpWallDist);

            int tex_x = int(wall_x * float(texture_wall_size));

            float ceilingTextureX = (wall_x - floor(wall_x)) * texture_wall_size;

            // add floor

           // lines.append(sf::Vertex(sf::Vector2f((float)x, (float)groundPixel), floorColor, sf::Vector2f(385, 129)));

            if (tile == 'r')
            {
                //ceilingPixel = int(-heightTile * wallHeight * (1.0 - cameraHeight) + screenHeight * 0.5);

                lines.append(sf::Vertex(
                    sf::Vector2f((float)x, (float)groundPixel * 1.1),
                    sf::Color::Transparent,
                    sf::Vector2f((float)(floorTextureCoords.x + ceilingTextureX), (float)floorTextureCoords.y)));
            }
            else
            {
                lines.append(sf::Vertex(
                    sf::Vector2f((float)x, (float)groundPixel + heightTile + verticalLookOffset),
                    currentLevel.floorColor,
                    sf::Vector2f((float)(floorTextureCoords.x), (float)floorTextureCoords.y)));
            }

            groundPixel = float(wallHeight * cameraHeight + screenHeight * 0.5);



            lines.append(sf::Vertex(
                sf::Vector2f((float)x, (float)groundPixel + verticalLookOffset),
                currentLevel.floorColor,
                sf::Vector2f((float)(floorTextureCoords.x + ceilingTextureX), (float)(floorTextureCoords.y + texture_wall_size - 1))));


            // add ceiling         


            get_textures(ceilingTextureNum, get_ceiling_tile(mapPos.x, mapPos.y));
            get_textures(wallTextureNum, tile);

            sf::Vector2i ceilingTextureCoords(
                ceilingTextureNum * texture_wall_size % texture_size,
                ceilingTextureNum * texture_wall_size / texture_size * texture_wall_size
            );

            lines.append(sf::Vertex(
                sf::Vector2f((float)x, (float)ceilingPixel + verticalLookOffset),
                currentLevel.color,
                sf::Vector2f((float)(ceilingTextureCoords.x), (float)ceilingTextureCoords.y)));


            if (cameraHeight >= 0.66)
                ceilingPixel = float((-wallHeight * heightTile) * (1.0 - cameraHeight) + screenHeight * 0.5);
            else
                ceilingPixel = float((-wallHeight * heightTile) * (1.0 - cameraHeight) + screenHeight * 0.5);

            lines.append(sf::Vertex(
                sf::Vector2f((float)x, (float)ceilingPixel + verticalLookOffset),
                currentLevel.color,
                sf::Vector2f((float)(ceilingTextureCoords.x + ceilingTextureX), (float)(ceilingTextureCoords.y + texture_wall_size - 1))));
        }


        if (perpWallDist < RENDER_DISTANCE)
        {
            // calculate lowest and highest pixel to fill in current line                  
            float drawStart = ceilingPixel + verticalLookOffset;
            float drawEnd = groundPixel + verticalLookOffset;

            // get position of the wall texture in the full texture
            //get_textures(wallTextureNum, tile);

            sf::Vector2i texture_coords(
                wallTextureNum * texture_wall_size % texture_size,
                wallTextureNum * texture_wall_size / texture_size * texture_wall_size
            );

            // calculate where the wall was hit
            float wall_x;
            if (horizontal)
            {
                wall_x = rayPos.y + perpWallDist * rayDir.y;

                // currentLevel.color.r /= 4;
                // currentLevel.color.g /= 4;
                // currentLevel.color.b /= 4;
            }
            else
            {
                wall_x = rayPos.x + perpWallDist * rayDir.x;
            }
            wall_x -= floor(wall_x);
            // get x coordinate on the wall texture
            int tex_x = int(wall_x * float(texture_wall_size));

            // flip texture if we see it on the other side of us, this prevents a mirrored effect for the texture
            if ((horizontal && rayDir.x <= 0) || (!horizontal && rayDir.y >= 0))
            {
                tex_x = texture_wall_size - tex_x - 1;
            }

            texture_coords.x += tex_x;

            // illusion of shading by making horizontal walls darker
            currentLevel.color = sf::Color::White;

            currentLevel.color.r /= wallShading;
            currentLevel.color.g /= wallShading;
            currentLevel.color.b /= wallShading;

            for (const auto& sprite : currentLevel.spriteObjects) {
                if (!sprite.isActive) continue;

                float dx = sprite.position.x - mapPos.x;
                float dy = sprite.position.y - mapPos.y;
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < sprite.lightRadius) {
                    float brightness = 1.0f - (distance / sprite.lightRadius); // Diminishing light
                    currentLevel.color.r = std::min(255, int(currentLevel.color.r + 100 * brightness));
                    currentLevel.color.g = std::min(255, int(currentLevel.color.g + 100 * brightness));
                    currentLevel.color.b = std::min(255, int(currentLevel.color.b + 100 * brightness));
                }
            }


            for (const auto& sprite : currentLevel.spriteObjects) 
            {

                for (const auto& sprite : currentLevel.spriteObjects)
                {
                    sf::Vector2f spritePos = sprite.position - player.position;

                    invDet = 1.0f / (player.plane.x * player.direction.y - player.direction.x * player.plane.y);

                    transformX = invDet * (player.direction.y * spritePos.x - player.direction.x * spritePos.y);
                    transformY = invDet * (-player.plane.y * spritePos.x + player.plane.x * spritePos.y);

                    if (transformY <= 0.5 || transformY > SPRITE_RENDER_DISTANCE) break;

                    spriteScreenX = int((screenWidth / 2) * (1 + transformX / transformY));

                    spriteHeight = abs(int(screenHeight / transformY));
                    spriteWidth = spriteHeight;

                    if (spriteHeight > maxSpriteSize) spriteHeight = maxSpriteSize;
                    if (spriteWidth > maxSpriteSize) spriteWidth = maxSpriteSize;

                    drawStartY = -spriteHeight / 2 + screenHeight / 2 + verticalLookOffset;
                    drawEndY = spriteHeight / 2 + screenHeight / 2 + verticalLookOffset;

                    drawStartX = -spriteWidth / 2 + spriteScreenX;
                    drawEndX = spriteWidth / 2 + spriteScreenX;

                    textureXStart = sprite.textureID * texture_wall_size;

                    for (float stripe = drawStartX; stripe < drawEndX; stripe++) {
                        if (stripe <= 0 || stripe >= screenWidth) break;
                        if (transformY > wallDistances[stripe]) break;

                        textureX = int(((stripe - drawStartX) * texture_wall_size) / spriteWidth) + textureXStart;

                        lines.append(sf::Vertex(
                            sf::Vector2f((float)stripe, (float)drawStartY),
                            sf::Color::White,
                            sf::Vector2f((float)textureX, 0)
                        ));
                        lines.append(sf::Vertex(
                            sf::Vector2f((float)stripe, (float)drawEndY),
                            sf::Color::White,
                            sf::Vector2f((float)textureX, (float)texture_wall_size)
                        ));

                    }
                }


                // add line to vertex buffer
                lines.append(sf::Vertex(
                    sf::Vector2f((float)x, (float)drawStart),
                    currentLevel.color,
                    sf::Vector2f((float)texture_coords.x, (float)texture_coords.y + 1)
                ));
                lines.append(sf::Vertex(
                    sf::Vector2f((float)x, (float)drawEnd),
                    currentLevel.color,
                    sf::Vector2f((float)texture_coords.x, (float)(texture_coords.y + texture_wall_size - 1))
                ));
            }
            window.draw(lines, state);
            lines.clear();
        }
        state.blendMode = sf::BlendAlpha;
    }
}

void GoToNextLevel(sf::RenderWindow& window, sf::RenderStates state, bool defaultPlayerStartingPos)
{
    currentLevel.AmbientSFX.stop();
    currentLevel.AmbientSFX2.stop();
    currentLevel.machineSFX.stop();
    free(currentLevel.map);
    free(currentLevel.heightMap);
    currentLevel.ID = currentLevel.ID_NextLevel;
    currentLevel.loadLevel(window, state, defaultPlayerStartingPos);
    levelChanged = false;
}

void SetBlackScreen(sf::RenderWindow& window, sf::Time seconds)
{
    sf::Clock clock;
    window.clear(sf::Color::Black);
    window.display();
    while (clock.getElapsedTime().asSeconds() != seconds.asSeconds())
    {

    }
}

int main()
{
    sf::Font font; // main hud text font
    sf::Vector2f size(player.playerSize, player.playerSize); // player collision box width and height
    sf::RenderStates state; // screen rendering state
    sf::VertexArray lines(sf::Lines, 18 * screenWidth); // lines used to draw walls and floors on the screen

    sf::SoundBuffer menuSoundBuffer;
    sf::Sound menuAmbientSFX;
    const char* ambientSFXAdress = "Data/Audio/Level0LightAmbience.mp3";

    static sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Data/Textures/background.jpg"))
    {
        printf("Error: Could not load background texture!\n");
    }

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    sf::Vector2u bgSize = backgroundTexture.getSize();
    backgroundSprite.setScale((float(screenWidth) / bgSize.x) * 1.5, (float(screenHeight) / bgSize.y) * 1.5);
    
    sf::Text fpsText("", font, 50); // text object for FPS counter
    sf::Clock clock; // timer
    int64_t frame_time_micro = 0; // time needed to draw frames in microseconds

    if (!font.loadFromFile("Data/Fonts/arial.ttf")) //loads font
    {
        std::cout << "Cannot open font arial.ttf!\n";
        return 1;
    }

    LoadUserSettingsData();

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "House Interior Sim", sf::Style::Titlebar | sf::Style::Close); // create window after loading everything up
    window.setSize(sf::Vector2u(screenWidth, screenHeight));
    window.setMouseCursorVisible(false); // Hide cursor for mouse look
    sf::Vector2i windowCenter(screenWidth / 2, screenHeight / 2);
    sf::Mouse::setPosition(windowCenter, window);
    prevMousePosition = windowCenter;

    //window.setFramerateLimit(60);
    sf::View view = window.getDefaultView();

    UIConstruct(font);

    RENDER_DISTANCE_COPY = RENDER_DISTANCE;

    while (window.isOpen())
    {
        // handle SFML events
        sf::Event event;

        if (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::LostFocus:
            {
                hasFocus = false;
                gameState = WINDOW_LOST_FOCUS;
                window.setMouseCursorVisible(true); // Show cursor

                break;
            }
            case sf::Event::GainedFocus:
            {
                hasFocus = true;
                gameState = IN_GAME;
                window.setMouseCursorVisible(false);
  
                break;
            }
            case sf::Event::Resized:
                // update the view to the new size of the window
                view.setSize({
                                        static_cast<float>(event.size.width),
                                        static_cast<float>(event.size.height)
                    });
                window.setView(view);
                ResizeMainMenuBackground(view);
                break;
            default:
                break;
            }
        }

        switch (gameState)
        {
        case MAIN_MENU:
        {
            MainMenu(window, state, font);

            break;
        }
        case IN_GAME:
        {
            menuAmbientSFX.stop();
            float dt = clock.restart().asSeconds();
            sf::Sprite sfSprite;

            UpdateFPS(fpsText, dt, frame_time_micro);

            sf::Vector2i currentMousePosition = sf::Mouse::getPosition(window);
            int deltaX = currentMousePosition.x - prevMousePosition.x;
            int deltaY = currentMousePosition.y - prevMousePosition.y;

            if (deltaX != 0) {
                float rotation = deltaX * mouseSensitivity;
                player.direction = player.Rotate(player.direction, rotation);
                player.plane = player.Rotate(player.plane, rotation);
            }

            if (deltaY != 0) {
                verticalLookOffset -= deltaY * verticalLookSensitivity;
                if (verticalLookOffset > maxVerticalLookUp) verticalLookOffset = maxVerticalLookUp;
                if (verticalLookOffset < -maxVerticalLookDown) verticalLookOffset = -maxVerticalLookDown;
            }

            // Reset mouse to the center of the window
            sf::Mouse::setPosition(windowCenter, window);
            prevMousePosition = windowCenter;


            if (levelChanged == true)
            {
                if (currentLevel.ID_NextLevel == 4)
                {
                    if (globalClock.getElapsedTime().asSeconds() >= 2)
                    {
                        SetBlackScreen(window, sf::seconds(1));
                        GoToNextLevel(window, state, true);
                    }
                }
                else
                {
                    GoToNextLevel(window, state, true);
                }
            }

            KeyboardInput(hasFocus, player, size, dt);
            //printf("X: %f Y: %f ", player.position.x, player.position.y);
            //system("cls");

            if (cameraHeight > 0.67 && !keyboardPressed)
                cameraHeight -= 0.1 * GRAVITY_ACCELERATION * dt;

            if (currentLevel.ID == 4 && globalClock.getElapsedTime().asSeconds() >= 10 && (player.position.x >= 1.0 && player.position.x < 2.0) &&
                (player.position.y >= 1.0 && player.position.y <= 2.0))
            {
                SetBlackScreen(window, sf::seconds(1));
                player.setPlayerNewPos(1.5, 4.5);
            }

            // Sort sprites by distance once per frame
            for (auto& sprite : currentLevel.spriteObjects) {
                float dx = sprite.position.x - player.position.x;
                float dy = sprite.position.y - player.position.y;
                sprite.distance = dx * dx + dy * dy;
            }
            std::sort(currentLevel.spriteObjects.begin(), currentLevel.spriteObjects.end(), [](const CurrentLevel::SpriteObject& a, const CurrentLevel::SpriteObject& b) {
                return a.distance > b.distance;
                });


            window.draw(backgroundSprite);
            Raycasting(window, state, lines);
            
            window.draw(fpsText);
            frame_time_micro += clock.getElapsedTime().asMicroseconds();

            break;
        }
        case PAUSE:
        {
            PauseMenu(window, font, event, view, prevMousePosition, windowCenter);
            window.setMouseCursorVisible(true);


            break;
        }
        case GAME_OVER:
        {
            break;
        }
        case WINDOW_LOST_FOCUS:
        {
            break;
        }
        default:
            break;
        }

        window.display();
    }


    return 0;
}
