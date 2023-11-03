#pragma once
#include <vector>
#include "Config.h"
#include "Selector.h"
#include "Block.h"
#include "Delegate.h"

class LevelManager
{
public:
    static LevelManager& Get();
    void LoadLevel(int levelToLoad);
    void UnloadLevel();

    void Update(float dt);





    void Render();
    bool IsLoaded() const { return m_loaded; }
    void Transform(int localX, int localY, float* worldX, float* worldY);
    void Transform(float worldX, float worldY, int* localX, int* localY);
    void MoveSelector(int dx, int dy);

    void HoldBlock();
    void ReleaseBlock();
    void ChangePosition(int startX, int startY, int endX, int endY, int tileNum);
    bool CanFall(int x, int y);
    void CheckNeighbors(int x, int y, int tileID);


    CDelegate OnLevelCleared;

private:
    std::vector<int> m_gridData;
    std::vector<Block*> m_activeBlocks;
    std::vector<Block*> m_deletedBlocks;

    int m_offsetX = 0;
    int m_offsetY = 0;
    int m_width = 0;
    int m_height = 0;
    int m_cellWidth = 0;
    int m_cellHeight = 0;
    int m_totalCell = 0;
    int m_currentLevelDebris = 0;
    bool m_loaded = false;
    size_t m_backgroundTiles = 0;
    size_t m_whiteFont = 0;
    Selector m_selector;
    bool m_holding = false;
    float m_fallingSpeed;

    Block* m_selectedBlock;

    LevelManager() = default;
    LevelManager(const LevelManager& other) = default;
    ~LevelManager() = default;

    int GetIndexFromPosition(int x, int y) const;
    void GetLocalPosition(int index, int width, int* outX, int* outY) const;
    void ToWorld(const int localX, const int localY, float* worldX, float* worldY) const;
    void FindStartingLocation(int* x, int* y);
    void GetLocalPosition(int index, int* outX, int* outY) const;
    Block* FindBlockAt(int x, int y);
    void Erase(Block* block);
    void UpdateActiveBlocks(float dt);
    void UpdateSelector(float dt);
    void RemoveDeletedBlocks();

    void OnBlockDestroyed(const BlockEvent& blockEvent);
};