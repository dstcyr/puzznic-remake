#pragma once
#include <vector>
#include "Config.h"
#include "Delegate.h"
#include "Block.h"
#include "Grid.h"

class LevelManager : public Grid
{
public:
    static LevelManager& Get();
    void LoadLevel(int levelToLoad);
    void UnloadLevel();
    void Update(float dt);
    void Render();
    bool IsLoaded() const { return m_loaded; }

    CDelegate OnLevelCleared;


    int FindGapFrom(float x, float y, int dx, int dy, const std::vector<Block*>& blocks);
    void GetBlocksOnTopOf(float x, float y, std::vector<Block*>& blocks);
    void MoveTile(int x1, int y1, int x2, int y2, int ID);
    bool CanFall(float x, float y);
    bool CanFall(Block* block);
    bool CanMove(float x, float y, int dx, int dy);
    void Snap(float x, float y, float* wx, float* wy);


private:
    std::vector<Block*> m_activeBlocks;
    bool m_loaded = false;
    size_t m_backgroundTiles = 0;
    size_t m_whiteFont = 0;

    LevelManager() = default;
    LevelManager(const LevelManager& other) = default;
    ~LevelManager() = default;

    void FindStartingLocation(int* x, int* y);
    void RenderGrid();
    Block* GetBlockAt(int x, int y);
    bool GetTile(int idx);
};