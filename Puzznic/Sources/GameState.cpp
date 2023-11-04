#include "GameState.h"
#include "LevelManager.h"
#include "SaveGame.h"
#include "Engine.h"

void GameState::OnEnter()
{
    if (!LevelManager::Get().IsLoaded())
    {
        int levelToLoad = SaveGame::level;
        LevelManager::Get().LoadLevel(levelToLoad);
        LevelManager::Get().OnLevelCleared.Bind(this, &GameState::OnLevelCleared);

        m_levelClearedElapsed = 0.0f;
        m_levelCleared = false;
    }
}

void GameState::OnUpdate(float dt)
{
    if (m_levelCleared)
    {
#if CHANGE_ON_LEVEL_CLEARED
        m_levelClearedElapsed += dt;
        if (m_levelClearedElapsed > 2.0f)
        {
            SaveGame::NextLevel();
            Engine::SetState("game");
        }

        return;
#endif
    }

    LevelManager::Get().Update(dt);
}

void GameState::OnRender()
{
    LevelManager::Get().Render();
}

void GameState::OnExit()
{
    LevelManager::Get().UnloadLevel();
}

void GameState::OnLevelCleared(const Event& e)
{
    m_levelCleared = true;
    m_levelClearedElapsed = 0.0f;
}
