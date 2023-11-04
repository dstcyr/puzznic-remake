#pragma once
#include <vector>
#include "Config.h"
#include "Delegate.h"
#include "Block.h"

class Grid
{
public:
    void Clear();
    void Transform(int localX, int localY, float* worldX, float* worldY) const;
    void Transform(float worldX, float worldY, int* localX, int* localY) const;

protected:
    std::vector<int> m_gridData;

    int m_offsetX = 0;
    int m_offsetY = 0;
    int m_width = 0;
    int m_height = 0;
    int m_cellWidth = 0;
    int m_cellHeight = 0;
    int m_totalCell = 0;

    int GetIndexFromPosition(int x, int y) const;
    int GetIndexFromPosition(float x, float y) const;
    void GetLocalPosition(int index, int width, int* outX, int* outY) const;
    void ToWorld(const int localX, const int localY, float* worldX, float* worldY) const;
    void GetLocalPosition(int index, int* outX, int* outY) const;
    void GetWorldPosition(int idx, float* worldX, float* worldY);
};