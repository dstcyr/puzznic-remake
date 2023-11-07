#include "Block.h"
#include "Engine.h"
#include "MathUtils.h"
#include "Level.h"
#include "Log.h"
#include "VectorHelper.h"
#include "Collision.h"

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

void PlacedBlock::Render()
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

void PlacedBlock::Update(float dt)
{
    Block::Update(dt);

    Fall(dt);

    // Cannot teleport while falling
    if (m_fallingInterp.moving) return;

    if (Engine::GetKeyDown(KEY_LEFT))
    {
        Teleport(-1);
    }

    if (Engine::GetKeyDown(KEY_RIGHT))
    {
        Teleport(1);
    }

    if (Engine::GetKeyDown(KEY_UP))
    {
    }

    if (Engine::GetKeyDown(KEY_DOWN))
    {
    }

    if (Engine::GetKeyDown(KEY_DEBUG))
    {

    }
}













/////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Teleports the block instantly in the given direction if nothing blocks it.
/// </summary>
/// <param name="direction">right/left direciton</param>
void PlacedBlock::Teleport(int direction)
{
    // Get the block's center position. This helps when looking at the block's
    // position in the grid.
    float centerX, centerY;
    GetCenter(&centerX, &centerY);

    // Get the grid space coordinates
    int gridX, gridY;
    Level::Get().Transform(centerX, centerY, &gridX, &gridY);

    // Move in direction using the grid's coordinates
    gridX += direction;

    // Check for collisions
    const int idx = Level::Get().GetIndex(gridX, gridY);
    bool hitWall = Level::Get().CellIsWall(idx);
    bool hitBlock = Level::Get().CellHasBlock(idx);

    if (!hitWall && !hitBlock)
    {
        // Get the pixel destination:
        float pixelX, pixelY;
        Level::Get().Transform(gridX, gridY, &pixelX, &pixelY);

        // Set the final position:
        SetPosition(pixelX, pixelY);
        OnBlockTeleported.Invoke<BlockEvent>(this);
    }
}

void PlacedBlock::Fall(float dt)
{


    //////// if (!m_fallingInterp.moving)
    //////// {
    ////////     if (CanFall())
    ////////     {
    ////////         // Initialize falling
    ////////         m_fallingInterp.moving = true;
    ////////         // m_fallingInterp.time = 0.0f;
    ////////         // m_fallingInterp.start = m_y;
    ////////         // m_fallingInterp.speed = INTERP_SPEED;
    ////////         // 
    ////////         // float centerX, centerY;
    ////////         // GetCenter(&centerX, &centerY);
    ////////         // 
    ////////         // int gridX, gridY;
    ////////         // Level::Get().Transform(centerX, centerY, &gridX, &gridY);
    ////////         // 
    ////////         // float endX, endY;
    ////////         // Level::Get().Transform(gridX, gridY + 1, &endX, &endY);
    ////////         // m_fallingInterp.end = endY;
    ////////     }
    //////// }
    //////// 
    //////// if (m_fallingInterp.moving)
    //////// {
    //////    float centerX, centerY;
    //////    GetCenter(&centerX, &centerY);
    //////    int gridX, gridY;
    //////    Level::Get().Transform(centerX, centerY, &gridX, &gridY);
    //////    const int idx = Level::Get().GetIndex(gridX, gridY + 1);

    //////    // if (!Level::Get().CellHasBlock(idx))
    //////    // if(!CanFall())

    //////    bool collideWithWall = false;
    //////    if (Level::Get().CellIsWall(idx))
    //////    {
    //////        float wx, wy;
    //////        Level::Get().Transform(gridX, gridY + 1, &wx, &wy);

    //////        collideWithWall = Engine::CheckRects(
    //////            m_x, m_y, BLOCK_SIZE, BLOCK_SIZE,
    //////            wx, wy, BLOCK_SIZE, BLOCK_SIZE
    //////        );
    //////    }

    //////    if (!CanFall())
    //////    {
    //////        if (Level::Get().Collides(this) || collideWithWall)
    //////        {
    //////            m_fallingInterp.moving = false;
    //////        }
    //////    }
    //////    else
    //////    {
    //////        m_fallingInterp.moving = true;
    //////    }
    //////    
    //////    if (m_fallingInterp.moving)
    //////    {
    //////        m_y += 50.0f * dt;
    //////    }

    //////    // if(Level::Get().Collides(this) || collideWithWall)
    //////    // {
    //////    //     m_fallingInterp.moving = false;
    //////    // }
    //////    // else
    //////    // {
    //////        // if (m_fallingInterp.time < m_fallingInterp.speed)
    //////        // {
    //////        //     // Update the falling interpolation
    //////        //     float time = m_fallingInterp.time;
    //////        //     float start = m_fallingInterp.start;
    //////        //     float end = m_fallingInterp.end;
    //////        //     float speed = m_fallingInterp.speed;
    //////        //     m_y = Engine::Linear(time, start, end, speed);
    //////        //     m_fallingInterp.time += dt;
    //////        
    //////        // m_fallingInterp.moving = true;
    //////        // m_y += 50.0f * dt;

    //////        // }
    //////        // else
    //////        // {
    //////        //     // The interpolation is over
    //////        //     m_y = m_fallingInterp.end;
    //////        //     m_fallingInterp.moving = false;
    //////        // }
    //////    // }
    //////// }
}

bool Block::CanFall()
{
    // Get the block's center position. This helps when looking at the block's
    // position in the grid.
    float centerX, centerY;
    GetCenter(&centerX, &centerY);

    // Get the grid space coordinates
    int gridX, gridY;
    Level::Get().Transform(centerX, centerY, &gridX, &gridY);
    gridY += 1;

    // Check for collisions
    const int idx = Level::Get().GetIndex(gridX, gridY);
    bool hitWall = Level::Get().CellIsWall(idx);
    bool hitBlock = Level::Get().CellHasBlock(idx);

    return !hitWall && !hitBlock;
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

void Platform::StartMoving()
{
}

void Platform::Pause()
{
    m_inPause = true;
    m_pauseElapsed = 0.0f;
    m_Interp.moving = false;
}

void HorizontalPlatform::Update(float dt)
{
    if (m_inPause)
    {
        UpdatePause(dt);
    }
    else if (m_Interp.moving)
    {
        UpdateInterp(&m_x, m_direction, 0, dt);
    }
    else
    {
        const int idx = Level::Get().GetIndexInDirection(this, m_direction, 0);

        if (Level::Get().CellIsWall(idx) || Level::Get().CellHasBlock(idx))
        {
            m_direction = -m_direction;
        }
        else
        {
            StartMoving();
        }
    }
}

void HorizontalPlatform::StartMoving()
{
    m_Interp.moving = true;
    m_Interp.time = 0.0f;
    m_Interp.start = m_x;
    m_Interp.end = m_x + BLOCK_SIZE * m_direction;
}

void Platform::UpdatePause(float dt)
{
    m_pauseElapsed += dt;
    if (m_pauseElapsed >= 0.5f)
    {
        m_pauseElapsed = 0.0f;
        m_inPause = false;
        StartMoving();
    }
}

void Platform::InterpolationEnd(int dx, int dy)
{
    const int idx = Level::Get().GetIndexInDirection(this, dx, dy);

    if (Level::Get().CellIsWall(idx) || Level::Get().CellHasBlock(idx))
    {
        Pause();
        m_direction = -m_direction;
    }
    else
    {
        StartMoving();
    }
}

void Platform::UpdateInterp(float* currentValue, int dx, int dy, float dt)
{
    if (m_Interp.time < m_Interp.speed)
    {
        *currentValue = Engine::Linear(m_Interp.time, m_Interp.start, m_Interp.end, m_Interp.speed);
        m_Interp.time += dt;
    }
    else
    {
        *currentValue = m_Interp.end;
        InterpolationEnd(dx, dy);
    }
}

void VerticalPlatform::Update(float dt)
{
    if (m_inPause)
    {
        UpdatePause(dt);
    }
    else if (m_Interp.moving)
    {
        UpdateInterp(&m_y, 0, m_direction, dt);
    }
    else
    {
        const int idx = Level::Get().GetIndexInDirection(this, 0, m_direction);

        if (Level::Get().CellIsWall(idx))
        {
            m_direction = -m_direction;
        }
        else
        {
            StartMoving();
        }
    }
}

void VerticalPlatform::StartMoving()
{
    m_Interp.moving = true;
    m_Interp.time = 0.0f;
    m_Interp.start = m_y;
    m_Interp.end = m_y + BLOCK_SIZE * m_direction;
}
