#include "SaveGame.h"
#include "Config.h"

int SaveGame::highScore = 50000;
int SaveGame::retry = STARTING_RETRY;
int SaveGame::level = START_AT_LEVEL;
int SaveGame::score = 0;

CFile SaveGame::m_saveGameFile;

void SaveGame::CheckHighScore()
{
    if (score > highScore)
    {
        highScore = score;
    }

    Save();
}

void SaveGame::Save()
{
    m_saveGameFile.Seek(0);
    m_saveGameFile.WriteInt(highScore);
    m_saveGameFile.Save("arkanoid.sav");
    m_saveGameFile.Release();
}

void SaveGame::Load()
{
    retry = STARTING_RETRY;
    level = START_AT_LEVEL;
    score = 0;

    if (CFile::Exists("puzznic.sav"))
    {
        m_saveGameFile.Load("puzznic.sav");
        highScore = m_saveGameFile.ReadInt();
        m_saveGameFile.Release();
    }
    else
    {
        highScore = 50000;
    }
}

void SaveGame::NextLevel()
{
    level++;
}
