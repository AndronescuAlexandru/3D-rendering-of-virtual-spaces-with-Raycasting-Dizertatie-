#include "Sprites.h"

bool PointInsideRect(const sf::Vector2f& point, const sf::FloatRect& rect)
{
    return point.x >= rect.left && point.x <= rect.left + rect.width &&
        point.y >= rect.top && point.y <= rect.top + rect.height;
}

bool RayIntersectsRect(const sf::Vector2f& rayPos, const sf::Vector2f& rayDir, const sf::FloatRect& rect, sf::Vector2f& intersection)
{
    float t = (rect.left - rayPos.x) / rayDir.x;
    if (t > 0 && PointInsideRect(rayPos + t * rayDir, rect))
    {
        intersection = rayPos + t * rayDir;
        return true;
    }
    return false;
}