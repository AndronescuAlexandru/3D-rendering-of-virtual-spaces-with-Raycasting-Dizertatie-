#include "FileOperations.h"
#include "Maps.h"
#include "Player.h"

extern CurrentLevel currentLevel;
extern Player player;
extern bool noSaveFile;
extern int RENDER_DISTANCE;
extern int screenWidth;
extern int screenHeight;
extern float menuSpriteScaleX;
extern float menuSpriteScaleY;

bool is_empty(std::fstream& File)
{
    return File.peek() == std::fstream::traits_type::eof();
}

void LoadSaveFile()
{
    std::fstream saveFile("Data/UserData/SaveFile.sav");

    if (!is_empty(saveFile))
    {
        saveFile >> currentLevel.ID;
        saveFile >> player.position.x;
        saveFile >> player.position.y;
        noSaveFile = false;
    }
    else
        noSaveFile = true;

    saveFile.close();
}

void WriteSaveFile()
{
    std::ofstream saveFile("Data/UserData/SaveFile.sav", std::ofstream::trunc);

    saveFile << currentLevel.ID << "\t";
    saveFile << player.position.x << "\t";
    saveFile << player.position.y << "\t";
    saveFile.close();

}

void WriteToUserSettings(std::ofstream& file)
{
    // opens UserSettings.dat file, delets its contents and then writes the new data

    file << RENDER_DISTANCE << "\t";
    file << screenHeight << "\t";
    file << screenWidth << "\t";
    file << menuSpriteScaleX << "\t";
    file << menuSpriteScaleY << "\t";
    file << master_sound_volume << "\t";

    file.close();
}

void LoadUserSettingsData()
{
    std::fstream userSettingsData("Data/UserData/UserSettings.dat");

    if (is_empty(userSettingsData)) // if the file is empty then adds the default data
    {
        userSettingsData << RENDER_DISTANCE << "\t";
        userSettingsData << screenHeight << "\t";
        userSettingsData << screenWidth << "\t";
        userSettingsData << menuSpriteScaleX << "\t";
        userSettingsData << menuSpriteScaleY << "\t";
        userSettingsData << master_sound_volume << "\t";
    }
    else
    {
        userSettingsData >> RENDER_DISTANCE;
        userSettingsData >> screenHeight;
        userSettingsData >> screenWidth;
        userSettingsData >> menuSpriteScaleX;
        userSettingsData >> menuSpriteScaleY;
        userSettingsData >> master_sound_volume;
    }
    userSettingsData.close();
}