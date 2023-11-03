#include "Selector.h"
#include "Engine.h"
#include "LevelManager.h"

void Selector::Initialize()
{
    m_selectorImage = Engine::LoadTexture("Assets/Images/selector.png");
}

void Selector::Render()
{
    if (!m_flashing)
    {
        Engine::DrawTexture(m_selectorImage,
            m_pixelX - (BLOCK_SIZE / 2.0f),
            m_pixelY - (BLOCK_SIZE / 2.0f)
        );
    }
}

void Selector::Update(float dt)
{
    m_holdingElapsed += dt;
    if (m_holdingElapsed > 0.05f)
    {
        m_holdingElapsed = 0.0f;
        m_flashing = !m_flashing;
    }
}

void Selector::ResetFlash()
{
    m_flashing = false;
    SetGridPosition(m_gridX, m_gridY);
}
