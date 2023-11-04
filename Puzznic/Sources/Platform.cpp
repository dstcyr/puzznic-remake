#include "Platform.h"
#include "Engine.h"
#include "Config.h"
#include "LevelManager.h"
#include "Log.h"

void Platform::Update(float dt)
{
    if (m_moving)
    {
        UpdateInterpolation(dt);
    }
    else
    {
        std::vector<Block*> blocks;
        LevelManager::Get().GetBlocksOnTopOf(m_x, m_y, blocks);

        int gapIdx = LevelManager::Get().FindGapFrom(m_x, m_y, m_directionX, m_directionY, blocks);
        if (gapIdx >= 0)
        {
            LOG(LL_DEBUG, "Gap: %d (%d blocks)", gapIdx, static_cast<int>(blocks.size()));
            SetupInterpolation(m_directionX, m_directionY);
            for (Block* block : blocks)
            {
                block->SetupInterpolation(m_directionX, m_directionY);
            }
        }
        else
        {
            m_directionX = -m_directionX;
            m_directionY = -m_directionY;
        }
    }
}

void Platform::Render()
{
    Engine::FillRect(
        static_cast<float>(m_x),
        static_cast<float>(m_y),
        static_cast<float>(BLOCK_SIZE),
        static_cast<float>(BLOCK_SIZE),
        NColor::DarkGray);

    Engine::DrawRect(
        static_cast<float>(m_x),
        static_cast<float>(m_y),
        static_cast<float>(BLOCK_SIZE),
        static_cast<float>(BLOCK_SIZE),
        NColor::Black);
}

void Platform::SetupInterpolation(int dx, int dy)
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
    LevelManager::Get().MoveTile(m_interpSourceX, m_interpSourceY, m_interpTargetX, m_interpTargetY, m_blockID);
}

void Platform::SetDirection(int dx, int dy)
{
    m_directionX = dx;
    m_directionY = dy;
}

void Platform::InterpolationCompleted()
{
    // Snap
    LevelManager::Get().Transform(m_interpTargetX, m_interpTargetY, &m_x, &m_y);
}
