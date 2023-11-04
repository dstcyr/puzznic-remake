#include "LevelManager.h"
#include "Collision.h"

void LevelManager::Update(float dt)
{
    for (Block* block : m_activePlatforms)
    {
        block->Update(dt);
    }

    for (Block* block : m_activeBlocks)
    {
        block->Update(dt);
    }

    for (int x = 0; x < m_width; x++)
    {
        for (int y = 0; y < m_height; y++)
        {
            Block* block = GetBlockAt(x, y);
            if (block && !block->IsMoving() && CanFall(block))
            {
                block->SetupInterpolation(0, 1);
            }
        }
    }
}

void LevelManager::RenderBlocks()
{
    for (Block* block : m_activeBlocks)
    {
        block->Render();
    }

    for (Block* block : m_activePlatforms)
    {
        block->Render();
    }
}

int LevelManager::FindGapFrom(float x, float y, int dx, int dy, const std::vector<Block*>& blocks)
{
    int gx, gy;
    Transform(x, y, &gx, &gy);

    gx += dx;
    gy += dy;
    int idx = GetIndexFromPosition(gx, gy);

    while (m_gridData[idx] != WALL_TILE)
    {
        if (m_gridData[idx] == EMPTY_TILE)
        {
            return idx;
        }

        Block* block = GetBlockAt(gx, gy);
        for (Block* other : blocks)
        {
            if (other != block)
            {
                return idx;
            }
        }

        gx += dx;
        gy += dy;
        idx = GetIndexFromPosition(gx, gy);
    }

    return -1;
}

void LevelManager::GetBlocksOnTopOf(float x, float y, std::vector<Block*>& blocks)
{
    int gx, gy;
    Transform(x, y, &gx, &gy);

    gy -= 1;
    int idx = GetIndexFromPosition(gx, gy);

    while (m_gridData[idx] != WALL_TILE)
    {
        if (m_gridData[idx] == EMPTY_TILE)
        {
            return;
        }

        Block* block = GetBlockAt(gx, gy);
        if (block != nullptr)
        {
            blocks.push_back(block);
        }

        gy -= 1;
        idx = GetIndexFromPosition(gx, gy);
    }
}

bool LevelManager::CanFall(float x, float y)
{
    int gx, gy;
    Transform(x, y, &gx, &gy);

    gy += 1;
    int idx = GetIndexFromPosition(gx, gy);
    if (m_gridData[idx] == WALL_TILE)
    {
        return false;
    }

    Block* block = GetBlockAt(gx, gy);

    if (block != nullptr)
    {
        float otherX, otherY;
        block->GetPosition(&otherX, &otherY);

        return !Engine::CheckRects(
            x, y, BLOCK_SIZE, BLOCK_SIZE,
            otherX, otherY, BLOCK_SIZE, BLOCK_SIZE
        );
    }

    return true;
}

bool LevelManager::CanFall(Block* block)
{
    float x, y;
    block->GetPosition(&x, &y);

    int gx, gy;
    Transform(x, y, &gx, &gy);

    gy += 1;
    int idx = GetIndexFromPosition(gx, gy);

    //if (m_gridData[idx] == WALL_TILE)
    if (m_gridData[idx] != EMPTY_TILE)
    {
        return false;
    }

    Block* other = GetBlockAt(gx, gy);

    if (other != nullptr)
    {
        float otherX, otherY;
        other->GetPosition(&otherX, &otherY);

        return !Engine::CheckRects(
            x, y, BLOCK_SIZE, BLOCK_SIZE,
            otherX, otherY, BLOCK_SIZE, BLOCK_SIZE
        );
    }

    return true;
}

bool LevelManager::CanMove(float x, float y)
{
    //int gx, gy;
    //Transform(x, y, &gx, &gy);
    //gx += dx;
    //gy += dy;

    //int idx = GetIndexFromPosition(gx, gy);
    //return m_gridData[idx] != WALL_TILE;

    float rightX = x + BLOCK_SIZE;
    float rightY = y + (BLOCK_SIZE / 2.0f);
    int rightIdx = GetIndexFromPosition(rightX, rightY);
    if (GetTile(rightIdx) == WALL_TILE)
    {
        return false;
    }

    return true;
}


Block* LevelManager::GetBlockAt(int x, int y)
{
    for (Block* block : m_activeBlocks)
    {
        float wx, wy;
        block->GetPosition(&wx, &wy);

        int gx, gy;
        Transform(wx, wy, &gx, &gy);

        if (gx == x && gy == y)
        {
            return block;
        }
    }

    return nullptr;
}

