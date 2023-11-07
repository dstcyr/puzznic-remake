#include "Block.h"
#include "Engine.h"
#include "MathUtils.h"
#include "Level.h"
#include "Log.h"
#include "VectorHelper.h"

void Block::Initialize()
{
    const float speed = 1.0f;
    m_horizontalMovements.current = m_x;
    m_horizontalMovements.end = m_x;
    m_horizontalMovements.speed = speed;
    m_verticalMovements.current = m_y;
    m_verticalMovements.end = m_y;
    m_verticalMovements.speed = speed;
}

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
    Update(dt, m_horizontalMovements);
    Update(dt, m_verticalMovements);

    if (m_horizontalMovements.moving)
    {
        m_x = m_horizontalMovements.current;
    }

    if (m_verticalMovements.moving)
    {
        m_y = m_verticalMovements.current;
    }
}

bool Block::IsMoving()
{
    return m_horizontalMovements.moving;
}

void Block::SetID(int ID)
{
    m_blockID = ID;
}

bool Block::InstantMove(int dx, int dy)
{
    // Get the current position center point:
    float cx, cy;
    GetCenter(&cx, &cy);






    float endX, endY;
    GetDestination(dx, dy, &endX, &endY);

    if (HitWall(endX, endX))
    {
        return false;
    }

    m_x = endX;
    m_y = endY;
    OnBlockInstantMove.Invoke<BlockEvent>(this);
    return true;
}

bool Block::MoveHorizontal(int direction)
{
    if (!m_horizontalMovements.moving)
    {
        float ex, ey;
        GetDestination(direction, 0, &ex, &ey);

        if (HitWall(ex, ey) || HitBlock(ex, ey))
        {
            return false;
        }

        InitInterp(m_horizontalMovements, m_x, ex);
        return true;
    }

    return false;
}

bool Block::MoveVertical(int direction)
{
    if (!m_verticalMovements.moving)
    {
        float ex, ey;
        GetDestination(0, direction, &ex, &ey);

        if (HitWall(ex, ey))
        {
            return false;
        }

        InitInterp(m_verticalMovements, m_y, ey);
        return true;
    }

    return false;
}

void Block::Update(float dt, BlockInterpolation& interp)
{
    if (interp.moving)
    {
        if (interp.time < interp.speed)
        {
            interp.current = Engine::Linear(interp.time, interp.start, interp.end, interp.speed);
            interp.time += dt;
        }
        else
        {
            interp.current = interp.end;
            interp.moving = false;
        }
    }
}

void Block::InitInterp(BlockInterpolation& interp, float start, float end)
{
    interp.moving = true;
    interp.time = 0.0f;
    interp.start = start;
    interp.end = end;
}

void Block::GetDestination(int dirX, int dirY, float* endX, float* endY)
{
    float cx = 0.0f, cy = 0.0f;
    GetCenter(&cx, &cy);

    int lx, ly;
    Level::Get().Transform(cx, cy, &lx, &ly);
    Level::Get().Transform(lx + dirX, ly + dirY, endX, endY);
}

void Block::GetDestination(int* dx, int* dy)
{
    float ex, ey;
    if (m_horizontalMovements.moving)
    {
        ex = m_horizontalMovements.end;
    }
    else
    {
        ex = m_x;
    }

    if (m_verticalMovements.moving)
    {
        ey = m_verticalMovements.end;
    }
    else
    {
        ey = m_y;
    }

    *dx = 0;
    *dy = 0;
    float cex, cey;
    Entity::GetCenter(ex, ey, &cex, &cey);
    Level::Get().Transform(cex, cey, dx, dy);
}

bool Block::HitWall(float x, float y)
{
    float cex, cey;
    Entity::GetCenter(x, y, &cex, &cey);

    int lx, ly;
    Level::Get().Transform(cex, cey, &lx, &ly);

    int idx = Level::Get().GetIndex(lx, ly);
    return Level::Get().CellIsWall(idx);
}

bool Block::HitBlock(float x, float y)
{
    float cex, cey;
    Entity::GetCenter(x, y, &cex, &cey);

    int lx, ly;
    Level::Get().Transform(cex, cey, &lx, &ly);

    int idx = Level::Get().GetIndex(lx, ly);
    return Level::Get().CellHasBlockOrDestination(idx);
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

    int dx, dy;
    GetDestination(&dx, &dy);
    float ex, ey;
    Level::Get().Transform(dx, dy, &ex, &ey);

    Engine::DrawCircle(
        ex + (BLOCK_SIZE / 2.0f),
        ey + (BLOCK_SIZE / 2.0f),
        10.0f,
        NColor::DarkBlue
    );
}

void DebugBlock::Update(float dt)
{
    Block::Update(dt);

    if (Engine::GetKeyDown(KEY_RIGHT))
    {
        InstantMove(1, 0);
    }

    if (Engine::GetKeyDown(KEY_LEFT))
    {
        InstantMove(-1, 0);
    }

    if (Engine::GetKeyDown(KEY_DOWN))
    {
        InstantMove(0, 1);
    }

    if (Engine::GetKeyDown(KEY_UP))
    {
        InstantMove(0, -1);
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

    Engine::DrawCircle(
        m_x + (BLOCK_SIZE / 2.0f),
        m_y + (BLOCK_SIZE / 2.0f),
        10.0f,
        NColor::LightBlue
    );

    float ex = m_horizontalMovements.end;
    float ey = m_y;

    Engine::DrawCircle(
        ex + (BLOCK_SIZE / 2.0f),
        ey + (BLOCK_SIZE / 2.0f),
        10.0f,
        NColor::Red
    );
}

void Platform::Update(float dt)
{
    Block::Update(dt);

    LOG(LL_DEBUG, "Number of blocks: %d", m_childBlocks.size());

    for (Block* block : m_childBlocks)
    {
        if (!block->IsMoving())
        {
            block->SetX(m_x);
        }
    }

    if (m_movingHorizontal)
    {
        if (!m_horizontalMovements.moving)
        {
            for (Block* b : m_childBlocks)
            {
                b->OnBlockInstantMove.Clear();
            }

            m_childBlocks.clear();

            Level::Get().GetBlockOnTop(this, m_childBlocks);

            for (Block* b : m_childBlocks)
            {
                b->OnBlockInstantMove.Bind(this, &Platform::OnBlockInstantMove);
            }

            bool moving = MoveHorizontal(m_direction);
            if (!moving)
            {
                m_direction = -m_direction;
                moving = MoveHorizontal(m_direction);
            }
        }
    }
    else
    {
        if (!m_verticalMovements.moving)
        {
            if (!MoveVertical(m_direction))
            {
                m_direction = -m_direction;
                MoveVertical(m_direction);
            }
        }
    }
}

void Platform::Start(bool horizontal)
{
    //Level::Get().GetBlockOnTop(this, m_childBlocks);
    m_movingHorizontal = horizontal;

    if (m_movingHorizontal)
    {
        for (Block* b : m_childBlocks)
        {
            b->OnBlockInstantMove.Clear();
        }

        m_childBlocks.clear();

        Level::Get().GetBlockOnTop(this, m_childBlocks);

        for (Block* b : m_childBlocks)
        {
            b->OnBlockInstantMove.Bind(this, &Platform::OnBlockInstantMove);
        }

        bool moving = MoveHorizontal(m_direction);
        if (!moving)
        {
            m_direction = -m_direction;
            moving = MoveHorizontal(m_direction);
        }

        // if (moving)
        // {
        //     for (Block* block : m_childBlocks)
        //     {
        //         block->MoveHorizontal(m_direction);
        //     }
        // }
    }
    else
    {
        if (!m_verticalMovements.moving)
        {
            if (!MoveVertical(m_direction))
            {
                m_direction = -m_direction;
                MoveVertical(m_direction);
            }
        }
    }
}

void Platform::OnBlockInstantMove(const BlockEvent& e)
{
    Engine::Remove(m_childBlocks, e.block);
}
