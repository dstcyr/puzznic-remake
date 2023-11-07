#include "Entity.h"
#include "Config.h"

void Entity::Initialize()
{
}

void Entity::Update(float dt)
{
}

void Entity::Render()
{
}

void Entity::SetPosition(float x, float y)
{
    m_x = x;
    m_y = y;
}

void Entity::SetX(float x)
{
    m_x = x;
}

void Entity::SetY(float y)
{
    m_y = y;
}

void Entity::GetPosition(float* x, float* y)
{
    *x = m_x;
    *y = m_y;
}

void Entity::GetCenter(float* x, float* y)
{
    *x = m_x + (BLOCK_SIZE / 2.0f);
    *y = m_y + (BLOCK_SIZE / 2.0f);
}

void Entity::GetCenter(float x, float y, float* cx, float* cy)
{
    *cx = x + (BLOCK_SIZE / 2.0f);
    *cy = y + (BLOCK_SIZE / 2.0f);
}
