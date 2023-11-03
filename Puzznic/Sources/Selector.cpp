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
        Engine::DrawTexture(m_selectorImage, m_worldX - (BLOCK_SIZE / 2.0f), m_worldY - (BLOCK_SIZE / 2.0f));
    }
}

void Selector::Update(float dt, bool holding)
{
    if (holding)
    {
        m_holdingElapsed += dt;
        if (m_holdingElapsed > 0.05f)
        {
            m_holdingElapsed = 0.0f;
            m_flashing = !m_flashing;
        }
    }
    else
    {
        m_flashing = false;
    }
}

void Selector::GetPosition(int* x, int* y)
{
    *x = m_x;
    *y = m_y;
}

void Selector::SetGridPosition(int x, int y)
{
    m_x = x;
    m_y = y;
    LevelManager::Get().Transform(m_x, m_y, &m_worldX, &m_worldY);
}

void Selector::SetPixelPosition(float x, float y)
{
    m_worldX = x;
    m_worldY = y;
    LevelManager::Get().Transform(m_worldX, m_worldY, &m_x, &m_y);
}
