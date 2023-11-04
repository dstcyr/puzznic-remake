#include "LevelManager.h"
#include "tinyxml2.h"
#include <string>
#include <sstream>
#include "Engine.h"
#include "Log.h"
#include "AnimatedBlock.h"
#include "Platform.h"
#include "Collision.h"

using namespace tinyxml2;

LevelManager& LevelManager::Get()
{
    static LevelManager levelManager;
    return levelManager;
}

void LevelManager::LoadLevel(int levelToLoad)
{
    m_backgroundTiles = Engine::LoadTexture("Assets/Images/background.png");
    std::string levelFilename = "Assets/Levels/Round" + std::to_string(levelToLoad) + ".tmx";

    XMLDocument doc;
    if (doc.LoadFile(levelFilename.c_str()) == XML_SUCCESS)
    {
        XMLNode* mapNode = doc.FirstChild()->NextSibling();
        XMLElement* mapElement = mapNode->ToElement();
        m_offsetX = LEVEL_OFFSET_X;
        m_offsetY = LEVEL_OFFSET_Y;
        m_width = mapElement->IntAttribute("width");
        m_height = mapElement->IntAttribute("height");
        m_cellWidth = mapElement->IntAttribute("tilewidth");
        m_cellHeight = mapElement->IntAttribute("tileheight");
        m_totalCell = m_width * m_height;

        XMLNode* layerNode = mapNode->FirstChild();
        while (layerNode)
        {
            XMLElement* layerElement = layerNode->ToElement();
            std::string valStr = layerElement->Value();
            if (valStr == "layer")
            {
                XMLNode* dataNode = layerNode->FirstChild();
                XMLElement* dataElement = dataNode->ToElement();
                const char* data = dataElement->GetText();

                std::string currentToken;
                std::istringstream inputStream(data);
                while (std::getline(inputStream, currentToken, ','))
                {
                    if (!currentToken.empty())
                    {
                        int tileNum = std::stoul(currentToken);
                        m_gridData.push_back(tileNum);
                        //// if (tileNum < 4)
                        //// {
                        ////     m_gridData.push_back(tileNum);
                        //// }
                        //// else
                        //// {
                        ////     m_gridData.push_back(EMPTY_TILE);
                        //// }
                        //// 
                        //// 
                        //// 
                        if (tileNum >= 4 && tileNum <= 11)
                        {
                            float worldX, worldY;
                            GetWorldPosition(static_cast<int>(m_gridData.size() - 1), &worldX, &worldY);

                            AnimatedBlock* newBlock = new AnimatedBlock();
                            newBlock->SetID(tileNum);
                            newBlock->Initialize();
                            newBlock->SetPosition(worldX, worldY);
                            m_activeBlocks.push_back(newBlock);
                        }
                        else if (tileNum == 12)
                        {
                            float worldX, worldY;
                            GetWorldPosition(static_cast<int>(m_gridData.size() - 1), &worldX, &worldY);

                            Platform* newBlock = new Platform();
                            newBlock->SetID(tileNum);
                            newBlock->Initialize();
                            newBlock->SetPosition(worldX, worldY);
                            m_activeBlocks.push_back(newBlock);
                        }
                    }
                }
            }

            layerNode = layerNode->NextSibling();
        }
    }

    m_loaded = true;

    int sx, sy;
    FindStartingLocation(&sx, &sy);
    m_whiteFont = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "white18", 18, NColor::White);
}

void LevelManager::UnloadLevel()
{
    Clear();
    m_loaded = false;

    for (Block* block : m_activeBlocks)
    {
        delete block;
    }

    m_activeBlocks.clear();
}

void LevelManager::Update(float dt)
{
    for (Block* block : m_activeBlocks)
    {
        block->Update(dt);
    }

    int y = m_height - 1;
}

void LevelManager::Render()
{
    for (Block* block : m_activeBlocks)
    {
        block->Render();
    }

    RenderGrid();



    for (int x = 0; x < m_width; x++)
    {
        for (int y = 0; y < m_height; y++)
        {
            Block* block = GetBlockAt(x, y);
            if (block && !block->IsMoving() && CanFall(block))
            {
                float xx, yy;
                block->GetPosition(&xx, &yy);
                Engine::DrawCircle(xx + 32, yy + 32, 10.0f, NColor::Yellow);
                block->SetupInterpolation(0, 1);
            }
        }
    }


    //for (int y = m_height - 1; y >= 0; y--)
    //{
    //    bool found = false;

    //    for (int x = 0; x < m_width; x++)
    //    {
    //        float xx, yy;
    //        Transform(x, y, &xx, &yy);
    //        Engine::DrawLine(-2000.0f, yy, 2000.0f, yy, NColor::Yellow);

    //        int idx = GetIndexFromPosition(x, y);
    //        if (GetTile(idx) != EMPTY_TILE)
    //        {
    //            Block* block = GetBlockAt(x, y);
    //            if (block && !block->IsMoving() && CanFall(block))
    //            {
    //                found = true;
    //                Engine::DrawCircle(xx + 32, yy + 32, 10.0f, NColor::Yellow);
    //            }

    //        }
    //    }

    //    if (found)
    //    {
    //        return;
    //    }
    //}
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
        // if (block != nullptr)
        // {
        //     blocks.push_back(block);
        // }

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

void LevelManager::MoveTile(int x1, int y1, int x2, int y2, int ID)
{
    int sourceIdx = GetIndexFromPosition(x1, y1);
    int targetIdx = GetIndexFromPosition(x2, y2);
    m_gridData[targetIdx] = ID;
    m_gridData[sourceIdx] = EMPTY_TILE;
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

bool LevelManager::CanMove(float x, float y, int dx, int dy)
{
    int gx, gy;
    Transform(x, y, &gx, &gy);
    gx += dx;
    gy += dy;

    int idx = GetIndexFromPosition(gx, gy);
    return m_gridData[idx] != WALL_TILE;
}

void LevelManager::RenderGrid()
{
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            int idx = GetIndexFromPosition(x, y);
            int tile = m_gridData[idx];

            Rect<int> src = { 0, 0, m_cellWidth, m_cellHeight };
            Rect<float> dst = {
                static_cast<float>(x * m_cellWidth + m_offsetX),
                static_cast<float>(y * m_cellHeight + m_offsetY),
                static_cast<float>(m_cellWidth),
                static_cast<float>(m_cellHeight)
            };

            Engine::DrawString(std::to_string(idx), m_whiteFont, dst.x, dst.y);
            if (tile >= WALL_TILE && tile <= SIZE_BACKGROUND_TILE)
            {
                src.x = m_cellWidth * (tile - 1);
                src.y = 0;
                Engine::DrawTexture(m_backgroundTiles, src, dst, 0.0f, false, false, NColor::White);
            }
            else if (tile != 0)
            {
#if SHOW_DEBUG_GRID
                //Engine::FillRect(x * m_cellWidth + m_offsetX, y * m_cellHeight + m_offsetY, m_cellWidth, m_cellHeight, NColor::DarkRed);
                Engine::DrawRect(x * m_cellWidth + m_offsetX - 1, y * m_cellHeight + m_offsetY - 1, m_cellWidth - 2, m_cellHeight - 2, NColor::Red);
                Engine::DrawRect(x * m_cellWidth + m_offsetX, y * m_cellHeight + m_offsetY, m_cellWidth, m_cellHeight, NColor::Red);
                Engine::DrawRect(x * m_cellWidth + m_offsetX + 1, y * m_cellHeight + m_offsetY + 1, m_cellWidth - 2, m_cellHeight - 2, NColor::Red);
#endif
            }
            else
            {
#if SHOW_DEBUG_GRID
                Engine::DrawRect(x * m_cellWidth + m_offsetX, y * m_cellHeight + m_offsetY, m_cellWidth, m_cellHeight, NColor::DarkBrown);
#endif
            }
        }
    }
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

bool LevelManager::GetTile(int idx)
{
    if (idx >= 0 && idx <= m_totalCell - 1)
    {
        return m_gridData[idx];
    }

    return INVALID_TILE;
}

void LevelManager::Snap(float x, float y, float* wx, float* wy)
{
    int gx, gy;
    Transform(x, y, &gx, &gy);
    Transform(gx, gy, wx, wy);
}

void LevelManager::FindStartingLocation(int* x, int* y)
{
    for (int py = 0; py < m_height; py++)
    {
        for (int px = 0; px < m_width; px++)
        {
            int idx = GetIndexFromPosition(px, py);
            int tile = m_gridData[idx];
            if (tile == EMPTY_TILE)
            {
                *x = px;
                *y = py;
                return;
            }
        }
    }

    *x = 0;
    *y = 0;
}
