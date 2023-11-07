#pragma once
#include <string>
#include <vector>

class Block;

class Level
{
public:
    static Level& Get()
    {
        static Level level;
        return level;
    }

    void Load(const std::string& filename);
    void Clear();
    void Update(float dt);
    void Render();

    void Transform(int localX, int localY, float* worldX, float* worldY) const;
    void Transform(float worldX, float worldY, int* localX, int* localY) const;

    int GetIndexInDirection(float startX, float startY, int dx, int dy) const;
    Block* CellHasBlock(int index) const;
    Block* CellHasBlockOrDestination(int index) const;
    Block* GetBlock(int index) const;
    Block* GetBlockAndDestination(int index) const;
    bool CellIsBlockDestination(int index) const;
    bool CellIsWall(int index) const;
    bool Collides(Block* block) const;
    void GetBlockOnTop(Block* block, std::vector<Block*>& blocks);
    int GetIndex(int x, int y) const;

private:
    std::vector<int> m_LevelData;
    std::vector<Block*> m_activeBlocks;

    int m_offsetX = 0;
    int m_offsetY = 0;
    int m_width = 0;
    int m_height = 0;
    int m_cellWidth = 0;
    int m_cellHeight = 0;
    int m_totalCell = 0;
    size_t m_backgroundTiles = 0;
    size_t m_whiteFont = 0;

    int GetData(int x, int y) const;
    int GetData(int index) const;
    void GetGridPosition(int index, int* outX, int* outY) const;
    void ToWorld(const int localX, const int localY, float* worldX, float* worldY) const;
    void GetWorldPosition(int idx, float* worldX, float* worldY) const;

    void RenderGrid();
    void RenderBlocks();
    void UpdateBlocks(float dt);
};
