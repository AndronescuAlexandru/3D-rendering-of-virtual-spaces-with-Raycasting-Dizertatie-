#pragma once
#ifndef TEXTURES_H
#define TEXTURES_H


#include <unordered_map>
#include <SFML/Graphics.hpp>

// list of wall texture types, in order as they appear in the full texture

enum class TexturesLevel1 {
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

enum class TexturesLevel0 {
    Lamp,
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
    HotelWall,
    HotelWall2,
};

// valid wall types and their texture for the world map

const std::unordered_map<char, TexturesLevel0> level0_wallTypes
{
    {'#', TexturesLevel0::HotelWall},
    {'@', TexturesLevel0::HotelWallWithDoor},
    {'$', TexturesLevel0::HotelWallElevator},
    {'%', TexturesLevel0::HotelWallPainting},
    {'^', TexturesLevel0::ElevatorInsideFront},
    {'&', TexturesLevel0::ElevatorInsideBack},
    {'!', TexturesLevel0::ElevatorInsideSides},
    {'*', TexturesLevel0::RoomWall},
    {']', TexturesLevel0::RoomWallWithDoor},
    {'W', TexturesLevel0::HotelWall2},
    {',', TexturesLevel0::RoomFloor},
    {':', TexturesLevel0::RoomCeiling},
    {'.', TexturesLevel0::HotelFloor},
    {';', TexturesLevel0::HotelCeiling},
    {'_', TexturesLevel0::ElevatorInsideFloor},

};

const std::unordered_map<char, TexturesLevel1> level1_wallTypes
{
    {'#', TexturesLevel1::OfficeWall},
    {'.', TexturesLevel1::OfficeCarpet},
    {'@', TexturesLevel1::OfficeWallWithDoor},
    {'$', TexturesLevel1::OfficeWallElevatorDoor},
    {'%', TexturesLevel1::OfficeWallWithWindow},
    {'&', TexturesLevel1::OfficeWallWithRadiator},
    {'(', TexturesLevel1::ElevatorInsideFront},
    {')', TexturesLevel1::ElevatorInsideBack},
    {'!', TexturesLevel1::ElevatorInsideSides},
    {',', TexturesLevel1::ElevatorInsideFloor},

};

#endif // !TEXTURES_H