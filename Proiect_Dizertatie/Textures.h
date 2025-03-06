#pragma once
#ifndef TEXTURES_H
#define TEXTURES_H


#include <unordered_map>
#include <SFML/Graphics.hpp>

// list of wall texture types, in order as they appear in the full texture

enum class TexturesLevel0 {
    YellowWall,
    YellowCarpet,
    YellowWallRedDoor,
    NoClipYellowWall,  
};

enum class TexturesLevel4 {
    OfficeWall,
    OfficeCarpet,
    OfficeWallWithDoor,
    OfficeWallElevatorDoor,
    ElevatorInsideFront,
    ElevatorInsideBack,
    ElevatorInsideSides,
    ElevatorInsideFloor,
    OfficeWallWithWindow,
    OfficeWallWithRadiator,
};

enum class TexturesLevel1 {
    HotelWall,
    HotelWallWithDoor,
    HotelWallElevator,
    HotelWallPainting,
    ElevatorInsideFront,
    ElevatorInsideBack,
    ElevatorInsideSides,
    ElevatorInsideFloor,
    HotelFloor,
    HotelCeiling,
    RoomWall,
    RoomFloor,
    RoomCeiling,
    RoomWallWithDoor,
    HotelWall2Painting,
    HotelWall2,
};

// valid wall types and their texture for the world map

const std::unordered_map<char, TexturesLevel0> level0_wallTypes
{
    {'#', TexturesLevel0::YellowWall},
    {'@', TexturesLevel0::YellowWallRedDoor},
    {'*', TexturesLevel0::NoClipYellowWall},
    {'.', TexturesLevel0::YellowCarpet},
    
};

const std::unordered_map<char, TexturesLevel1> level1_wallTypes
{
    {'#', TexturesLevel1::HotelWall},
    {'@', TexturesLevel1::HotelWallWithDoor},
    {'$', TexturesLevel1::HotelWallElevator},
    {'%', TexturesLevel1::HotelWallPainting},
    {'^', TexturesLevel1::ElevatorInsideFront},
    {'&', TexturesLevel1::ElevatorInsideBack},
    {'!', TexturesLevel1::ElevatorInsideSides},
    {'*', TexturesLevel1::RoomWall},
    {']', TexturesLevel1::RoomWallWithDoor},
    {'W', TexturesLevel1::HotelWall2},
    {'P', TexturesLevel1::HotelWall2Painting},
    {',', TexturesLevel1::RoomFloor},
    {':', TexturesLevel1::RoomCeiling},
    {'.', TexturesLevel1::HotelFloor},
    {';', TexturesLevel1::HotelCeiling},
    {'_', TexturesLevel1::ElevatorInsideFloor},

};

const std::unordered_map<char, TexturesLevel4> levelTest_wallTypes
{
    {'#', TexturesLevel4::OfficeWall},
    {'.', TexturesLevel4::OfficeCarpet},
    {'@', TexturesLevel4::OfficeWallWithDoor},
    {'$', TexturesLevel4::OfficeWallElevatorDoor},
    {'%', TexturesLevel4::OfficeWallWithWindow},
    {'&', TexturesLevel4::OfficeWallWithRadiator},
    {'(', TexturesLevel4::ElevatorInsideFront},
    {')', TexturesLevel4::ElevatorInsideBack},
    {'!', TexturesLevel4::ElevatorInsideSides},
    {',', TexturesLevel4::ElevatorInsideFloor},

};

#endif // !TEXTURES_H