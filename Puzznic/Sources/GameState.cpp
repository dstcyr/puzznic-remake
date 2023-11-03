#include "GameState.h"
#include "LevelManager.h"
#include "SaveGame.h"
#include "Engine.h"

void GameState::OnEnter()
{
    if (!LevelManager::Get().IsLoaded())
    {
        int levelToLoad = SaveGame::round;
        LevelManager::Get().LoadLevel(levelToLoad);
    }
}

void GameState::OnUpdate(float dt)
{
    if (Engine::GetKeyDown(KEY_LEFT))
    {
        LevelManager::Get().MoveSelector(-1, 0);
    }
    else if (Engine::GetKeyDown(KEY_RIGHT))
    {
        LevelManager::Get().MoveSelector(1, 0);
    }
    else if (Engine::GetKeyDown(KEY_UP))
    {
        LevelManager::Get().MoveSelector(0, -1);
    }
    else if (Engine::GetKeyDown(KEY_DOWN))
    {
        LevelManager::Get().MoveSelector(0, 1);
    }
    
    if (Engine::GetKey(KEY_A))
    {
        LevelManager::Get().HoldBlock();
    }
    else
    {
        LevelManager::Get().ReleaseBlock();
    }

    LevelManager::Get().Update(dt);
}

void GameState::OnRender()
{
    LevelManager::Get().Render();
}

void GameState::OnExit()
{
}
