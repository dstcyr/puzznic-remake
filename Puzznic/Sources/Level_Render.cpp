#include "Level.h"
#include "Rect.h"
#include "Engine.h"
#include "Block.h"
#include "Config.h"

void Level::Render()
{
    RenderGrid();
    RenderBlocks();
}

void Level::RenderGrid()
{
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            int data = GetData(x, y);
            Rect<int> src = { 0, 0, m_cellWidth, m_cellHeight };
            Rect<float> dst = {
                static_cast<float>(x * m_cellWidth + m_offsetX),
                static_cast<float>(y * m_cellHeight + m_offsetY),
                static_cast<float>(m_cellWidth),
                static_cast<float>(m_cellHeight)
            };

            if (data >= WALL_TILE && data <= SIZE_BACKGROUND_TILE)
            {
                src.x = m_cellWidth * (data - 1);
                src.y = 0;
                Engine::DrawTexture(m_backgroundTiles, src, dst, 0.0f, false, false, NColor::White);
            }

            Engine::DrawRect(dst, NColor::Red);

            int idx = GetIndex(x, y);
            Engine::DrawString(std::to_string(idx), m_whiteFont, dst.x, dst.y);
        }
    }
}

void Level::RenderBlocks()
{
    for (Block* block : m_activeBlocks)
    {
        block->Render();
    }
}
