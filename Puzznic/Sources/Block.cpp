#include "Block.h"
#include "Engine.h"
#include "MathUtils.h"
#include "Level.h"
#include "Log.h"
#include "VectorHelper.h"

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

void Block::Update(float dt)
{
}

void Block::SetID(int ID)
{
    m_blockID = ID;
}

void DebugBlock::Render()
{
    Engine::FillRect(
        static_cast<float>(m_x),
        static_cast<float>(m_y),
        static_cast<float>(BLOCK_SIZE),
        static_cast<float>(BLOCK_SIZE),
        NColor::ReddishBrown);

    Engine::DrawRect(
        static_cast<float>(m_x),
        static_cast<float>(m_y),
        static_cast<float>(BLOCK_SIZE),
        static_cast<float>(BLOCK_SIZE),
        NColor::Black);

    Engine::DrawCircle(
        m_x + (BLOCK_SIZE / 2.0f),
        m_y + (BLOCK_SIZE / 2.0f),
        10.0f,
        NColor::Yellow
    );
}

void DebugBlock::Update(float dt)
{
    Block::Update(dt);
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

    Engine::DrawCircle(
        m_x + (BLOCK_SIZE / 2.0f),
        m_y + (BLOCK_SIZE / 2.0f),
        10.0f,
        NColor::LightBlue
    );
}

void Platform::Update(float dt)
{
}
