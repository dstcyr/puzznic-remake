#include "MovingBlock.h"
#include "LevelManager.h"
#include "Engine.h"
#include "MathUtils.h"

void MovingBlock::Initialize(int tileID)
{
    m_tileID = tileID;
    m_blockAnimation.Init("Assets/Images/background.png", 3, BLOCK_SIZE, BLOCK_SIZE);
    m_blockAnimation.AddClip("idle", 0, 1, 0.0f);
    m_blockAnimation.Play("idle", false);

    m_dirX = 0;
    m_dirY = -1;
}

void MovingBlock::Update(float dt)
{
    if (!m_moving)
    {
        if (CanMove())
        {
            SetupInterpolation(m_dirX, m_dirY);
        }
        else
        {
            m_dirX = -m_dirX;
            m_dirY = -m_dirY;
        }
    }

    UpdateInterpolation(dt);
}

void MovingBlock::UpdateInterpolation(float dt)
{
    if (m_moving)
    {
        if (m_interpTime < m_interpSpeed)
        {
            m_pixelX = Engine::Linear(m_interpTime, m_interpStartX, m_interpEndX, m_interpSpeed);
            m_pixelY = Engine::Linear(m_interpTime, m_interpStartY, m_interpEndY, m_interpSpeed);
            m_interpTime += dt;
        }
        else
        {
            m_moving = false;
            LevelManager::Get().ChangePosition(m_interpLocalX, m_interpLocalY, m_gridX, m_gridY, m_tileID);
        }
    }
}

bool MovingBlock::CanMove()
{
    if (LevelManager::Get().CanMoveInDirection(m_gridX, m_gridY, m_dirX, m_dirY))
    {
        return true;
    }

    return false;
}
