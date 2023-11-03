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
        float wx, wy;
        LevelManager::Get().Transform(m_x, m_y, &wx, &wy);
        Engine::DrawTexture(m_selectorImage, wx - (BLOCK_SIZE / 2.0f), wy - (BLOCK_SIZE / 2.0f));
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

void Selector::SetPosition(int x, int y)
{
    m_x = x;
    m_y = y;
}
