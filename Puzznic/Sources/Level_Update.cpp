#include "Level.h"
#include "Block.h"

void Level::Update(float dt)
{
    UpdateBlocks(dt);
}

void Level::UpdateBlocks(float dt)
{
    for (Block* block : m_activeBlocks)
    {
        block->Update(dt);
    }
}
