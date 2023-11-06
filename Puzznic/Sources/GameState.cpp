#include "GameState.h"
#include "SaveGame.h"
#include "Engine.h"
#include "Level.h"

void GameState::OnEnter()
{
    Level::Get().Load("Assets/Levels/Round0.tmx");
}

void GameState::OnUpdate(float dt)
{
    Level::Get().Update(dt);
}

void GameState::OnRender()
{
    Level::Get().Render();
}

void GameState::OnExit()
{
    Level::Get().Clear();
}