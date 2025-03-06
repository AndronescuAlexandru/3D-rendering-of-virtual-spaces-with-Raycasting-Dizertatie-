#include "Player.h"
#include "Maps.h"

Player player;

extern CurrentLevel currentLevel;
extern char GetTile(int x, int y, short level);
extern float GetHeight(int x, int y, short level);

Player::Player()
{
    position.x = 1.5;
    position.y = 1.5;
    direction.x = 0.0;
    direction.y = 1.0;
    plane.x = -0.66;
    plane.y = 0.0;
    playerSize = 0.375;
    playerMoveSpeed = 2;
    playerRotateSpeed = 3.0;
    sanity = 100;
}

Player::~Player()
{

}

void Player::setPlayerNewPos(float x, float y)
{
    position.x = x;
    position.y = y;
}

bool Player::CanMove(sf::Vector2f position, sf::Vector2f size)
{
    // create the corners of the rectangle
    sf::Vector2i upper_left(position - size / 2.0f);
    sf::Vector2i lower_right(position + size / 2.0f);

    if (upper_left.x < 0 || upper_left.y < 0 || lower_right.x >= currentLevel.MAP_WIDTH || lower_right.y >= currentLevel.MAP_HEIGHT)
    {
        return false; // out of map bounds
    }

    // loop through each map tile within the rectangle. The rectangle could be multiple tiles in size!
    for (int y = upper_left.y; y <= lower_right.y; y++)
    {
        for (int x = upper_left.x; x <= lower_right.x; x++)
        {
            if (GetTile(x, y, currentLevel.ID) != '.' && GetTile(x, y, currentLevel.ID) != ';' && GetTile(x, y, currentLevel.ID) != ',')
            {
                return false;
            }
        }
    }
    return true;
}

sf::Vector2f Player::Rotate(sf::Vector2f vec, float value)
{
    return sf::Vector2f(vec.x * std::cos(value) - vec.y * std::sin(value), vec.x * std::sin(value) + vec.y * std::cos(value));
}