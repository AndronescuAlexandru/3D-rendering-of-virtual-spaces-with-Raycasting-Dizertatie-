#pragma once
#ifndef MENUS_H
#define MENUS_H

#include <fstream>
#include "Constants.h"
#include "Maps.h"
#include "FileOperations.h"

extern int RENDER_DISTANCE;
extern int gameState;
extern int screenWidth, screenHeight;
extern unsigned int master_sound_volume;
extern float menuSpriteScaleX;
extern float menuSpriteScaleY;
extern bool debugMode;
extern bool hasFocus;
extern bool noSaveFile;

extern void WriteToUserSettings(std::ofstream& file);
extern void WriteSaveFile();
extern void Raycasting(sf::RenderWindow& window, sf::RenderStates state, sf::VertexArray& lines);

extern CurrentLevel currentLevel;

void UIConstruct(sf::Font font);
void MainMenu(sf::RenderWindow& window, sf::RenderStates& state, sf::Font font);
void PauseMenu(sf::RenderWindow& window, sf::Font font, sf::Event event, sf::View view, sf::Vector2i& prevMousePosition, sf::Vector2i& windowCenter);
void OptionsMenu(sf::RenderWindow& window, sf::Font font, sf::Sprite& menuBackgroundSprite);
void ExitMenu();

#endif