#include "Block.h"
#include "Engine.h"
#include "LevelManager.h"
#include "Log.h"
#include "MathUtils.h"

void Block::Initialize(int tileID)
{
    m_tileID = tileID;

    int frameCount = 9;
    int firstFrame = (tileID - 4) * frameCount;
    m_blockAnimation.Init("Assets/Images/blocks.png", frameCount, BLOCK_SIZE, BLOCK_SIZE);
    m_blockAnimation.AddClip("idle", firstFrame, 1, 0.0f);
    m_blockAnimation.AddClip("destroy", firstFrame, frameCount, 0.07f);
    m_blockAnimation.Play("idle", false);
    m_falling = false;
    m_destroyed = false;
    m_moving = false;
}

void Block::Render()
{
    m_blockAnimation.Render({
        static_cast<float>(m_pixelX - (BLOCK_SIZE / 2.0f)),
        static_cast<float>(m_pixelY - (BLOCK_SIZE / 2.0f)),
        static_cast<float>(BLOCK_SIZE),
        static_cast<float>(BLOCK_SIZE) }
    );
}

void Block::Update(float dt)
{
    if (m_destroyed && !m_destroyedAnimationCompleted)
    {
        if (!m_blockAnimation.Update(dt))
        {
            m_destroyedAnimationCompleted = true;
            OnBlockDestroyed.Invoke<BlockEvent>(m_tileID, m_gridX, m_gridY);
        }
    }

    UpdateInterpolation(dt);
}

void Block::UpdateInterpolation(float dt)
{
    if (!m_falling) return;
    if (m_destroyed) return;

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
            CheckFalling();

            if (!m_falling)
            {
                LevelManager::Get().CheckNeighbors(m_gridX, m_gridY, m_tileID);
            }
        }
    }
}

void Block::CheckFalling()
{
    if (m_destroyed) return;
    m_falling = LevelManager::Get().CanFall(m_gridX, m_gridY);
    if (m_falling)
    {
        SetupInterpolation(0, 1);
    }
}

void Block::SetupInterpolation(int dx, int dy)
{
    m_moving = true;
    m_interpTime = 0.0f;
    m_interpStartX = m_pixelX;
    m_interpStartY = m_pixelY;
    m_interpLocalX = m_gridX;
    m_interpLocalY = m_gridY;
    m_gridX += dx;
    m_gridY += dy;
    LevelManager::Get().Transform(m_gridX, m_gridY, &m_interpEndX, &m_interpEndY);
}

void Block::MoveTo(int x, int y)
{
    if (m_destroyed) return;
    if (m_falling) return;

    LevelManager::Get().ChangePosition(m_gridX, m_gridY, x, y, m_tileID);
    SetGridPosition(x, y);
    CheckFalling();

    if (!m_falling)
    {
        LevelManager::Get().CheckNeighbors(m_gridX, m_gridY, m_tileID);
    }
}

void Block::Destroy()
{
    m_destroyed = true;
    m_falling = false;
    m_destroyedAnimationCompleted = false;
    m_blockAnimation.Play("destroy", false);
}
