#include "Level.h"
#include "Log.h"
#include "Block.h"
#include "Collision.h"

int Level::GetIndex(int x, int y) const
{
    return static_cast<size_t>(x + m_width * y);
}

int Level::GetIndexInDirection(Block* block, int dx, float dy) const
{
    float centerX, centerY;
    block->GetCenter(&centerX, &centerY);

    int gridX, gridY;
    Level::Get().Transform(centerX, centerY, &gridX, &gridY);
    return Level::Get().GetIndex(gridX + dx, gridY + dy);
}

int Level::GetData(int x, int y) const
{
    int idx = GetIndex(x, y);
    return GetData(idx);
}

int Level::GetData(int index) const
{
    if (index >= 0 && index < m_LevelData.size())
    {
        int data = m_LevelData[index];
        return data;
    }

    int x, y;
    GetGridPosition(index, &x, &y);
    LOG(LL_ERROR, "Level::GetDataAt(%d, %d) out of bound", x, y);
    return -1;
}

void Level::GetGridPosition(int index, int* outX, int* outY) const
{
    *outY = static_cast<int>(index) / m_width;
    *outX = static_cast<int>(index) - m_width * *outY;
}

void Level::ToWorld(const int localX, const int localY, float* worldX, float* worldY) const
{
    *worldX = static_cast<float>(localX * m_cellWidth) + m_offsetX;
    *worldY = static_cast<float>(localY * m_cellHeight) + m_offsetY;
}

void Level::GetWorldPosition(int idx, float* worldX, float* worldY) const
{
    int gridX, gridY;
    GetGridPosition(idx, &gridX, &gridY);
    ToWorld(gridX, gridY, worldX, worldY);
}

void Level::Transform(int localX, int localY, float* worldX, float* worldY) const
{
    ToWorld(localX, localY, worldX, worldY);
}

void Level::Transform(float worldX, float worldY, int* localX, int* localY) const
{
    *localX = static_cast<int>((worldX - m_offsetX) / m_cellWidth);
    *localY = static_cast<int>((worldY - m_offsetY) / m_cellHeight);
}

Block* Level::CellHasBlock(int index) const
{
    int localX, localY;
    GetGridPosition(index, &localX, &localY);

    for (Block* other : m_activeBlocks)
    {
        float otherX, otherY;
        other->GetCenter(&otherX, &otherY);

        int ox, oy;
        Transform(otherX, otherY, &ox, &oy);
        if (ox == localX && oy == localY)
        {
            return other;
        }
    }

    return nullptr;
}


Block* Level::CellHasBlockOrDestination(int index) const
{
    int localX, localY;
    GetGridPosition(index, &localX, &localY);

    for (Block* other : m_activeBlocks)
    {
        float otherX, otherY;
        other->GetCenter(&otherX, &otherY);

        // int destX, destY;
        // other->GetDestination(&destX, &destY);

        int ox, oy;
        Transform(otherX, otherY, &ox, &oy);
        if ((ox == localX && oy == localY) /*|| (destX == localX && destY == localY)*/)
        {
            return other;
        }
    }

    return nullptr;
}


Block* Level::GetBlock(int index) const
{
    int localX, localY;
    GetGridPosition(index, &localX, &localY);

    for (Block* other : m_activeBlocks)
    {
        float otherX, otherY;
        other->GetCenter(&otherX, &otherY);

        int ox, oy;
        Transform(otherX, otherY, &ox, &oy);
        if (ox == localX && oy == localY)
        {
            return other;
        }
    }

    return nullptr;
}

Block* Level::GetBlockAndDestination(int index) const
{
    int localX, localY;
    GetGridPosition(index, &localX, &localY);

    for (Block* other : m_activeBlocks)
    {
        float otherX, otherY;
        other->GetCenter(&otherX, &otherY);

        // int destX, destY;
        // other->GetDestination(&destX, &destY);

        int ox, oy;
        Transform(otherX, otherY, &ox, &oy);
        if ((ox == localX && oy == localY) /*|| (destX == localX && destY == localY)*/)
        {
            return other;
        }
    }

    return nullptr;
}

bool Level::CellIsBlockDestination(int index) const
{
    int localX, localY;
    GetGridPosition(index, &localX, &localY);

    for (Block* other : m_activeBlocks)
    {
        // int destX, destY;
        // other->GetDestination(&destX, &destY);

        //if (destX == localX && destY == localY)
        // {
        //     return true;
        // }
    }

    return false;
}

bool Level::CellIsWall(int index) const
{
    return GetData(index) == WALL_TILE;
}

bool Level::Collides(Block* block) const
{
    float bx, by;
    block->GetPosition(&bx, &by);

    for (Block* other : m_activeBlocks)
    {
        if (other != block)
        {
            float ox, oy;
            other->GetPosition(&ox, &oy);
            return Engine::CheckRects(bx, by, BLOCK_SIZE, BLOCK_SIZE, ox, oy, BLOCK_SIZE, BLOCK_SIZE);
        }
    }

    return false;
}

void Level::GetBlockOnTop(Block* block, std::vector<Block*>& blocks)
{
    blocks.clear();

    float bx, by;
    block->GetCenter(&bx, &by);

    int gx, gy;
    Transform(bx, by, &gx, &gy);

    gy -= 1;

    if (gy >= 0)
    {
        int idx = GetIndex(gx, gy);

        while (gy >= 0 && (m_LevelData[idx] != WALL_TILE || m_LevelData[idx] != EMPTY_TILE))
        {
            Block* block = GetBlockAndDestination(idx);
            if (block != nullptr)
            {
                blocks.push_back(block);
            }
            else
            {
                return;
            }

            gy -= 1;
            idx = GetIndex(gx, gy);
        }
    }
}
