#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player
{
public:
    sf::Vector2f position; // coordinates in worldMap
    sf::Vector2f direction; // direction, relative to (0,0)
    sf::Vector2f plane; // 2d raycaster version of the camera plane,

    int sanity; // player sanity level

    float playerSize; // dimensions of player collision box, in tiles
    float playerMoveSpeed; // player movement speed in tiles per second
    float playerRotateSpeed; // player rotation speed in radians per second

    Player();
    void setPlayerNewPos(float x, float y);

    // check if player can move to given position without colliding with walls or
    // being outside of the map
    // player is constructed as a rectangle with position considered the middle of the rectangle
    bool CanMove(sf::Vector2f position, sf::Vector2f size);

    // rotates the player with given float value in radians and return the result
    sf::Vector2f Rotate(sf::Vector2f vec, float value);
    ~Player();

};

#endif // !PLAYER_H