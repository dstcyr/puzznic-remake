#include "Grid.h"

void Grid::Clear()
{
    m_gridData.clear();
}

void Grid::Transform(int localX, int localY, float* worldX, float* worldY) const
{
    ToWorld(localX, localY, worldX, worldY);
    //*worldX += (m_cellWidth / 2.0f);
    //*worldY += (m_cellHeight / 2.0f);
}

void Grid::Transform(float worldX, float worldY, int* localX, int* localY) const
{
    *localX = static_cast<int>((worldX - m_offsetX) / m_cellWidth);
    *localY = static_cast<int>((worldY - m_offsetY) / m_cellHeight);
}

int Grid::GetIndexFromPosition(int x, int y) const
{
    return x + m_width * y;
}

int Grid::GetIndexFromPosition(float x, float y) const
{
    int gridX, gridY;
    Transform(x, y, &gridX, &gridY);
    return GetIndexFromPosition(gridX, gridY);
}

void Grid::GetLocalPosition(int index, int width, int* outX, int* outY) const
{
    *outY = static_cast<int>(index) / width;
    *outX = static_cast<int>(index) - width * *outY;
}

void Grid::ToWorld(const int localX, const int localY, float* worldX, float* worldY) const
{
    *worldX = static_cast<float>(localX * m_cellWidth) + m_offsetX;
    *worldY = static_cast<float>(localY * m_cellHeight) + m_offsetY;
}

void Grid::GetLocalPosition(int index, int* outX, int* outY) const
{
    *outY = static_cast<int>(index) / m_width;
    *outX = static_cast<int>(index) - m_width * *outY;
}

void Grid::GetWorldPosition(int idx, float* worldX, float* worldY)
{
    int gridX, gridY;
    GetLocalPosition(idx, &gridX, &gridY);
    ToWorld(gridX, gridY, worldX, worldY);
}
