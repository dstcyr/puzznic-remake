#include "Block.h"
#include "Engine.h"
#include "LevelManager.h"
#include "MathUtils.h"

void Block::Render()
{
    Engine::FillRect(
        static_cast<float>(m_x),
        static_cast<float>(m_y),
        static_cast<float>(BLOCK_SIZE),
        static_cast<float>(BLOCK_SIZE),
        NColor::DarkPurple);

    Engine::DrawRect(
        static_cast<float>(m_x),
        static_cast<float>(m_y),
        static_cast<float>(BLOCK_SIZE),
        static_cast<float>(BLOCK_SIZE),
        NColor::Black);
}

void Block::SetID(int ID)
{
    m_blockID = ID;
}

void Block::SetupInterpolation(int dx, int dy)
{
    m_moving = true;
    m_interpTime = 0.0f;
    m_interpStartX = m_x;
    m_interpStartY = m_y;
    m_interpDirX = dx;
    m_interpDirY = dy;
    LevelManager::Get().Transform(m_x, m_y, &m_interpSourceX, &m_interpSourceY);

    m_interpTargetX = m_interpSourceX + dx;
    m_interpTargetY = m_interpSourceY + dy;
    LevelManager::Get().Transform(m_interpTargetX, m_interpTargetY, &m_interpEndX, &m_interpEndY);

    // Move collision
    //LevelManager::Get().MoveTile(m_interpSourceX, m_interpSourceY, m_interpTargetX, m_interpTargetY, m_blockID);
    LevelManager::Get().MarkTile(m_interpSourceX, m_interpSourceY, m_interpTargetX, m_interpTargetY, m_blockID);
}

void Block::UpdateInterpolation(float dt)
{
    if (m_interpTime < m_interpSpeed)
    {
        m_x = Engine::Linear(m_interpTime, m_interpStartX, m_interpEndX, m_interpSpeed);
        m_y = Engine::Linear(m_interpTime, m_interpStartY, m_interpEndY, m_interpSpeed);
        m_interpTime += dt;
    }
    else
    {
        InterpolationCompleted();
        m_moving = false;
    }
}

void Block::InterpolationCompleted()
{
    // Snap
    LevelManager::Get().Transform(m_interpTargetX, m_interpTargetY, &m_x, &m_y);

    LevelManager::Get().MarkTile(m_interpSourceX, m_interpSourceY, m_interpSourceX, m_interpSourceY, 0);
}
