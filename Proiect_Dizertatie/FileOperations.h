#pragma once
#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H
#include <fstream>

bool is_empty(std::fstream& File);
void LoadSaveFile();
void WriteSaveFile();
void WriteToUserSettings(std::ofstream& file);
void LoadUserSettingsData();

#endif // !FILEOPERATIONS_H

