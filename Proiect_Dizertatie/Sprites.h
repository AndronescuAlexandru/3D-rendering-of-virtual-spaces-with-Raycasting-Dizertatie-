#pragma once
#ifndef SPRITES_H
#define SPRITES_H
#include "Maps.h"

class Sprite
{
public:
    sf::Vector2f position;
    sf::Texture texture;

    Sprite(sf::Vector2f pos, const std::string& texturePath)
        : position(pos)
    {
        texture.loadFromFile(texturePath);
    }
};

#endif