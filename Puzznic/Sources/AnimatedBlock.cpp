#include "AnimatedBlock.h"
#include "Engine.h"
#include "Config.h"
#include "LevelManager.h"
#include "MathUtils.h"

void AnimatedBlock::Initialize()
{
    int frameCount = 9;
    int firstFrame = (m_blockID - 4) * frameCount;
    m_blockAnimation.Init("Assets/Images/blocks.png", frameCount, BLOCK_SIZE, BLOCK_SIZE);
    m_blockAnimation.AddClip("idle", firstFrame, 1, 0.0f);
    m_blockAnimation.AddClip("destroy", firstFrame, frameCount, 0.07f);
    m_blockAnimation.Play("idle", false);
}

void AnimatedBlock::Update(float dt)
{
    if (m_moving)
    {
        UpdateInterpolation(dt);
    }
    else
    {
        //if (LevelManager::Get().CanFall(m_x, m_y))
        //{
        //    SetupInterpolation(0, 1);
        //}
    }
}

void AnimatedBlock::Render()
{
    Engine::FillRect(
        static_cast<float>(m_x),
        static_cast<float>(m_y),
        static_cast<float>(BLOCK_SIZE),
        static_cast<float>(BLOCK_SIZE),
        NColor::DarkOliveGreen);

    Engine::DrawRect(
        static_cast<float>(m_x),
        static_cast<float>(m_y),
        static_cast<float>(BLOCK_SIZE),
        static_cast<float>(BLOCK_SIZE),
        NColor::Black);

    m_blockAnimation.Render({
        static_cast<float>(m_x),
        static_cast<float>(m_y),
        static_cast<float>(BLOCK_SIZE),
        static_cast<float>(BLOCK_SIZE)}
    );
}

void AnimatedBlock::UpdateInterpolation(float dt)
{
    if (m_interpTime < m_interpSpeed)
    {
        float wx = Engine::Linear(m_interpTime, m_interpStartX, m_interpEndX, m_interpSpeed);
        float wy = Engine::Linear(m_interpTime, m_interpStartY, m_interpEndY, m_interpSpeed);
        if (LevelManager::Get().CanMove(wx, wy, m_interpDirX, m_interpDirY))
        {
            m_x = wx;
            m_y = wy;
        }
        else
        {
            //InterpolationCompleted();
            // Snap
            LevelManager::Get().Snap(m_x, m_y, &m_x, &m_y);

            // Move collision
            //LevelManager::Get().MoveTile(m_interpSourceX, m_interpSourceY, m_interpTargetX, m_interpTargetY, m_blockID);

            m_moving = false;
            return;
        }

        m_interpTime += dt;
    }
    else
    {
        InterpolationCompleted();
        m_moving = false;
    }
}