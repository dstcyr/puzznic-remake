#pragma once
#include "Config.h"
#include "StateMachine.h"
#include "Delegate.h"

class GameState : public IState
{
public:
    void OnEnter() override;
    void OnUpdate(float dt) override;
    void OnRender() override;
    void OnExit() override;

private:
    size_t m_whiteFont = 0;
    size_t m_orangeFont = 0;
    bool m_levelCleared = false;
    float m_levelClearedElapsed = 0.0f;

    void OnLevelCleared(const Event& e);
};