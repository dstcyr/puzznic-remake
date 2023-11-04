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
                            newBlock->SetDirection(1, 0);
                            m_activePlatforms.push_back(newBlock);
                        }
                        else if (tileNum == 13)
                        {
                            float worldX, worldY;
                            GetWorldPosition(static_cast<int>(m_gridData.size() - 1), &worldX, &worldY);

                            Platform* newBlock = new Platform();
                            newBlock->SetID(tileNum);
                            newBlock->Initialize();
                            newBlock->SetPosition(worldX, worldY);
                            newBlock->SetDirection(0, 1);
                            m_activePlatforms.push_back(newBlock);
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

    for (Block* block : m_activePlatforms)
    {
        delete block;
    }

    m_activeBlocks.clear();
    m_activePlatforms.clear();
}

void LevelManager::Render()
{
    RenderBlocks();
    RenderGrid();
}

void LevelManager::MoveTile(int x1, int y1, int x2, int y2, int ID)
{
    int sourceIdx = GetIndexFromPosition(x1, y1);
    int targetIdx = GetIndexFromPosition(x2, y2);
    m_gridData[targetIdx] = ID;
    m_gridData[sourceIdx] = EMPTY_TILE;
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

            if (tile >= WALL_TILE && tile <= SIZE_BACKGROUND_TILE)
            {
                src.x = m_cellWidth * (tile - 1);
                src.y = 0;
                Engine::DrawTexture(m_backgroundTiles, src, dst, 0.0f, false, false, NColor::White);
            }
#if SHOW_DEBUG_GRID
            else if (tile >= 4 && tile <= 11)
            {
                Engine::DrawRect(x * m_cellWidth + m_offsetX - 1, y * m_cellHeight + m_offsetY - 1, m_cellWidth - 2, m_cellHeight - 2, NColor::Red);
                Engine::DrawRect(x * m_cellWidth + m_offsetX, y * m_cellHeight + m_offsetY, m_cellWidth, m_cellHeight, NColor::Red);
                Engine::DrawRect(x * m_cellWidth + m_offsetX + 1, y * m_cellHeight + m_offsetY + 1, m_cellWidth - 2, m_cellHeight - 2, NColor::Red);
            }
            else
            {
                Engine::DrawRect(x * m_cellWidth + m_offsetX, y * m_cellHeight + m_offsetY, m_cellWidth, m_cellHeight, NColor::DarkBrown);
            }

            Engine::DrawString(std::to_string(idx), m_whiteFont, dst.x, dst.y);
#endif
        }
    }
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
