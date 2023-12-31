#include "LevelManager.h"
#include "tinyxml2.h"
#include <string>
#include <sstream>
#include "Engine.h"
#include "Block.h"
#include "Log.h"
#include "MovingBlock.h"

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
        m_currentLevelDebris = levelToLoad % 4;
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
                            Block* newBlock = new Block();
                            newBlock->Initialize(tileNum);
                            newBlock->OnBlockDestroyed.Bind(this, &LevelManager::OnBlockDestroyed);

                            int x, y;
                            GetLocalPosition(static_cast<int>(m_gridData.size()) - 1, &x, &y);
                            newBlock->SetGridPosition(x, y);
                            m_activeBlocks.push_back(newBlock);
                        }
                        else if (tileNum == 12)
                        {
                            MovingBlock* newBlock = new MovingBlock();
                            newBlock->Initialize(tileNum);

                            int x, y;
                            GetLocalPosition(static_cast<int>(m_gridData.size()) - 1, &x, &y);
                            newBlock->SetGridPosition(x, y);
                            m_MovingBlocks.push_back(newBlock);
                        }
                    }
                }
            }

            layerNode = layerNode->NextSibling();
        }
    }

    m_loaded = true;

    m_selector.Initialize();

    int sx, sy;
    FindStartingLocation(&sx, &sy);
    m_selector.SetGridPosition(sx, sy);
    m_selectedBlock = nullptr;
    m_fallingSpeed = 0.0f;
    m_whiteFont = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "white18", 30, NColor::White);
}

void LevelManager::UnloadLevel()
{
    for (Block* block : m_activeBlocks)
    {
        delete block;
    }

    for (Block* block : m_MovingBlocks)
    {
        delete block;
    }

    m_activeBlocks.clear();
    m_MovingBlocks.clear();
    m_deletedBlocks.clear();
    m_gridData.clear();
    m_loaded = false;
}

void LevelManager::Render()
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
            else if (tile != 0)
            {
#if SHOW_DEBUG_GRID
                Engine::FillRect(x * m_cellWidth + m_offsetX, y * m_cellHeight + m_offsetY, m_cellWidth, m_cellHeight, NColor::DarkRed);
                Engine::DrawRect(x * m_cellWidth + m_offsetX, y * m_cellHeight + m_offsetY, m_cellWidth, m_cellHeight, NColor::Black);
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

    for (Block* block : m_activeBlocks)
    {
        block->Render();
    }

    for (Block* block : m_MovingBlocks)
    {
        block->Render();
    }

    m_selector.Render();

#if SHOW_DEBUG_GRID
    int x, y;
    m_selector.GetGridPosition(&x, &y);
    Engine::DrawString("{" + std::to_string(x) + ", " + std::to_string(y) + "}", m_whiteFont, 10.0f, 10.0f);
#endif
}


void LevelManager::Transform(int localX, int localY, float* worldX, float* worldY)
{
    ToWorld(localX, localY, worldX, worldY);
    *worldX += (m_cellWidth / 2.0f);
    *worldY += (m_cellHeight / 2.0f);
}

void LevelManager::Transform(float worldX, float worldY, int* localX, int* localY)
{
    *localX = static_cast<int>((worldX - m_offsetX) / m_cellWidth);
    *localY = static_cast<int>((worldY - m_offsetY) / m_cellHeight);
}

void LevelManager::MoveSelector(int dx, int dy)
{
    if (m_holding && dy != 0) return;

    int x, y;
    m_selector.GetGridPosition(&x, &y);
    int px = x + dx;
    int py = y + dy;

    if (px < 0 || px > m_width)
    {
        px = x;
    }

    if (py < 0 || py > m_height)
    {
        py = y;
    }

    int idx = GetIndexFromPosition(px, py);
    int tile = m_gridData[idx];

    if (tile == BACKGROUND_TILE || tile == SIZE_BACKGROUND_TILE || tile == WALL_TILE)
    {
        px = x;
        py = y;
    }

    if (m_holding && tile >= 4 && tile <= 12)
    {
        px = x;
        py = y;
    }

    m_selector.SetGridPosition(px, py);

    if (m_holding)
    {
        idx = GetIndexFromPosition(px, py);
        if (m_selectedBlock)
        {
            m_selectedBlock->MoveTo(px, py);
        }
    }
}

void LevelManager::HoldBlock()
{
    if (!m_selectedBlock)
    {
        int x, y;
        m_selector.GetGridPosition(&x, &y);

        int idx = GetIndexFromPosition(x, y);
        int tileNum = m_gridData[idx];
        m_holding = false;

        if (tileNum >= 4 && tileNum <= 11)
        {
            m_selectedBlock = FindBlockAt(x, y);
            m_holding = m_selectedBlock != nullptr;
        }
    }
}

void LevelManager::ReleaseBlock()
{
    if (m_selectedBlock)
    {
        m_selectedBlock = nullptr;
    }

    m_holding = false;
    m_selector.ResetFlash();
}

void LevelManager::ChangePosition(int startX, int startY, int endX, int endY, int tileNum)
{
    int startIdx = GetIndexFromPosition(startX, startY);
    int endIdx = GetIndexFromPosition(endX, endY);

    m_gridData[startIdx] = EMPTY_TILE;
    m_gridData[endIdx] = tileNum;
}

bool LevelManager::CanFall(int x, int y)
{
    int idx = GetIndexFromPosition(x, y + 1);
    if (idx < static_cast<int>(m_gridData.size()))
    {
        return m_gridData[idx] == EMPTY_TILE;
    }

    return false;
}

void LevelManager::CheckNeighbors(int x, int y, int tileID)
{
    std::vector<std::pair<int, int>> direction = { {x + 1, y}, {x - 1, y}, {x, y + 1}, {x, y - 1} };
    std::vector<std::string> directionNames = { "RIGHT", "LEFT", "DOWN", "UP" };
    LOG(LL_DEBUG, "FROM %d, %d => TRUE", x, y);

    int removeCount = 0;
    int i = 0;
    for (auto& location : direction)
    {
        int idx = GetIndexFromPosition(location.first, location.second);
        int tile = m_gridData[idx];

        if (tile == tileID)
        {
            LOG(LL_DEBUG, "%d, %d (%s) => TRUE", location.first, location.second, directionNames[i].c_str());
            Block* block = FindBlockAt(location.first, location.second);
            if (block)
            {
                block->Destroy();
                removeCount++;
            }
        }
        else
        {
            LOG(LL_DEBUG, "%d, %d (%s) => FALSE", location.first, location.second, directionNames[i].c_str());
        }

        i++;
    }

    if (removeCount > 0)
    {
        Block* block = FindBlockAt(x, y);
        if (block)
        {
            int idx = GetIndexFromPosition(x, y);
            block->Destroy();
        }
    }
}

bool LevelManager::CanMoveInDirection(int x, int y, int dx, int dy)
{
    int px = x + dx;
    int py = y + dy;
    int idx = GetIndexFromPosition(px, py);
    return m_gridData[idx] == EMPTY_TILE;
}

void LevelManager::Update(float dt)
{
    UpdateSelector(dt);
    UpdateActiveBlocks(dt);

    for (Block* block : m_MovingBlocks)
    {
        block->Update(dt);
    }

    RemoveDeletedBlocks();
}

int LevelManager::GetIndexFromPosition(int x, int y) const
{
    return x + m_width * y;
}

void LevelManager::GetLocalPosition(int index, int width, int* outX, int* outY) const
{
    *outY = static_cast<int>(index) / width;
    *outX = static_cast<int>(index) - width * *outY;
}

void LevelManager::Erase(Block* block)
{
    auto it = m_activeBlocks.begin();
    while (it != m_activeBlocks.end())
    {
        if (*it == block)
        {
            if (m_selectedBlock == *it)
            {
                ReleaseBlock();
            }

            int x, y;
            block->GetGridPosition(&x, &y);
            int idx = GetIndexFromPosition(x, y);
            m_gridData[idx] = EMPTY_TILE;

            m_activeBlocks.erase(it);
            return;
        }

        it++;
    }
}

void LevelManager::OnBlockDestroyed(const BlockEvent& blockEvent)
{
    Block* block = FindBlockAt(blockEvent.gridX, blockEvent.gridY);
    if (block)
    {
        int idx = GetIndexFromPosition(blockEvent.gridX, blockEvent.gridY);
        m_deletedBlocks.push_back(block);
    }
}

void LevelManager::UpdateActiveBlocks(float dt)
{
    if (m_activeBlocks.size() == 0)
    {
        OnLevelCleared.Invoke<Event>();
        return;
    }

    for (Block* block : m_activeBlocks)
    {
        block->Update(dt);

        if (!block->IsFalling())
        {
            // If the block is not falling,
            // check if it can fall right now
            block->CheckFalling();
        }
        else
        {
            // If the block is held and falling, 
            // the selector falls with it
            if (block != nullptr && block == m_selectedBlock && m_holding)
            {
                float bx, by;
                m_selectedBlock->GetPixelPosition(&bx, &by);
                m_selector.SetPixelPosition(bx, by);
            }
        }
    }
}

void LevelManager::UpdateSelector(float dt)
{
    if (m_holding)
    {
        m_selector.Update(dt);
        if (!m_selectedBlock)
        {
            m_holding = false;
            m_selector.ResetFlash();
        }
    }
}

void LevelManager::RemoveDeletedBlocks()
{
    if (m_deletedBlocks.size() > 0)
    {
        auto it = m_deletedBlocks.begin();
        while (it != m_deletedBlocks.end())
        {
            Block* block = *it;
            Erase(block);
            block->OnBlockDestroyed.Clear();
            it = m_deletedBlocks.erase(it);
        }
    }
}

Block* LevelManager::FindBlockAt(int x, int y)
{
    for (Block* block : m_activeBlocks)
    {
        int bx, by;
        block->GetGridPosition(&bx, &by);
        if (bx == x && by == y)
        {
            return block;
        }
    }

    return nullptr;
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

void LevelManager::ToWorld(const int localX, const int localY, float* worldX, float* worldY) const
{
    *worldX = static_cast<float>(localX * m_cellWidth) + m_offsetX;
    *worldY = static_cast<float>(localY * m_cellHeight) + m_offsetY;
}

void LevelManager::GetLocalPosition(int index, int* outX, int* outY) const
{
    *outY = static_cast<int>(index) / m_width;
    *outX = static_cast<int>(index) - m_width * *outY;
}