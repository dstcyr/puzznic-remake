#include "Level.h"
#include "tinyxml2.h"
#include "Engine.h"
#include <sstream>
#include "Block.h"

using namespace tinyxml2;

void Level::Load(const std::string& filename)
{
    m_backgroundTiles = Engine::LoadTexture("Assets/Images/background.png");
    m_whiteFont = Engine::LoadFont("Assets/Fonts/8bitwonder.ttf", "white18", 18, NColor::White);

    std::vector<Platform*> platforms;

    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) == XML_SUCCESS)
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
                        m_LevelData.push_back(tileNum);

                        float worldX, worldY;
                        GetWorldPosition(static_cast<int>(m_LevelData.size() - 1), &worldX, &worldY);

                        if (tileNum == 4)
                        {
                            DebugBlock* newBlock = new DebugBlock();
                            newBlock->SetID(tileNum);
                            newBlock->SetPosition(worldX, worldY);
                            newBlock->Initialize();
                            m_activeBlocks.push_back(newBlock);
                        }
                        if (tileNum > 4 && tileNum <= 11)
                        {
                            Block* newBlock = new Block();
                            newBlock->SetID(tileNum);
                            newBlock->SetPosition(worldX, worldY);
                            newBlock->Initialize();
                            m_activeBlocks.push_back(newBlock);
                        }

                        if (tileNum == 12)
                        {
                            Platform* newBlock = new Platform();
                            newBlock->SetID(tileNum);
                            newBlock->SetPosition(worldX, worldY);
                            newBlock->Initialize();
                            //newBlock->SetDirection(1, 0);
                            //newBlock->Start();
                            // newBlock->Start(true);
                            m_activeBlocks.push_back(newBlock);
                            platforms.push_back(newBlock);
                        }

                        if (tileNum == 13)
                        {
                            Platform* newBlock = new Platform();
                            newBlock->SetID(tileNum);
                            newBlock->SetPosition(worldX, worldY);
                            newBlock->Initialize();
                            //newBlock->SetDirection(0, -1);
                            //newBlock->Start();
                            // newBlock->Start(false);
                            m_activeBlocks.push_back(newBlock);
                            platforms.push_back(newBlock);
                        }
                    }
                }
            }

            layerNode = layerNode->NextSibling();
        }
    }


    for (Platform* p : platforms)
    {
        p->Start(true);
    }
}

void Level::Clear()
{
    m_LevelData.clear();
}
