#pragma once
#include "File.h"

class SaveGame
{
public:
    static int highScore;
    static int level;
    static int retry;
    static int score;

    static void CheckHighScore();
    static void Save();
    static void Load();
    static void NextLevel();

private:
    static CFile m_saveGameFile;
};