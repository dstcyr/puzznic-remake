#include "Block.h"
#include "Engine.h"
#include "LevelManager.h"
#include "Log.h"
#include "MathUtils.h"

void Block::Initialize(int tileID)
{
    m_tileID = tileID;

    int frameCount = 8;
    int firstFrame = (tileID - 4) * frameCount;
    m_blockAnimation.Init("Assets/Images/blocks.png", frameCount, BLOCK_SIZE, BLOCK_SIZE);
    m_blockAnimation.AddClip("idle", firstFrame, 1, 0.0f);
    m_blockAnimation.AddClip("destroy", firstFrame, frameCount, 0.07f);
    m_blockAnimation.Play("idle", false);
    m_falling = false;
    m_destroyed = false;
}

void Block::Render()
{
    m_blockAnimation.Render({
        static_cast<float>(m_worldX - (BLOCK_SIZE / 2.0f)),
        static_cast<float>(m_worldY - (BLOCK_SIZE / 2.0f)),
        static_cast<float>(BLOCK_SIZE),
        static_cast<float>(BLOCK_SIZE) }
    );
}

bool Block::Update(float dt)
{
    bool animResult = m_blockAnimation.Update(dt);
    if (!animResult && m_destroyed)
    {
        LOG(LL_DEBUG, "DELETE");
        return false;
    }

    UpdateInterpolation(dt);
    return true;
}

void Block::UpdateInterpolation(float dt)
{
    if (!m_falling) return;
    if (m_destroyed) return;

    if (m_interpTime < m_interpSpeed)
    {
        m_worldX = Engine::Linear(m_interpTime, m_interpStartX, m_interpEndX, m_interpSpeed);
        m_worldY = Engine::Linear(m_interpTime, m_interpStartY, m_interpEndY, m_interpSpeed);
        m_interpTime += dt;
    }
    else
    {
        LevelManager::Get().ChangePosition(m_interpLocalX, m_interpLocalY, m_localX, m_localY, m_tileID);
        CheckFalling();

        if (!m_falling)
        {
            LevelManager::Get().CheckNeighbors(m_localX, m_localY, m_tileID);
        }
    }
}

void Block::CheckFalling()
{
    m_falling = LevelManager::Get().CanFall(m_localX, m_localY);
    if (m_falling)
    {
        SetupInterpolation(0, 1);
    }
}

void Block::SetupInterpolation(int dx, int dy)
{
    m_interpTime = 0.0f;
    m_interpStartX = m_worldX;
    m_interpStartY = m_worldY;
    m_interpLocalX = m_localX;
    m_interpLocalY = m_localY;
    m_localX += dx;
    m_localY += dy;
    LevelManager::Get().Transform(m_localX, m_localY, &m_interpEndX, &m_interpEndY);
}

void Block::GetPosition(int* x, int* y)
{
    *x = m_localX;
    *y = m_localY;
}

void Block::SetPosition(int x, int y)
{
    if (m_destroyed) return;
    if (m_falling) return;

    m_localX = x;
    m_localY = y;
    LevelManager::Get().Transform(m_localX, m_localY, &m_worldX, &m_worldY);
    //CheckFalling();
}

void Block::MoveTo(int x, int y)
{
    if (m_destroyed) return;
    if (m_falling) return;

    LevelManager::Get().ChangePosition(m_localX, m_localY, x, y, m_tileID);
    SetPosition(x, y);
    CheckFalling();

    if (!m_falling)
    {
        LevelManager::Get().CheckNeighbors(m_localX, m_localY, m_tileID);
    }
}

void Block::Destroy()
{
    m_destroyed = true;
    m_blockAnimation.Play("destroy", false);
}
