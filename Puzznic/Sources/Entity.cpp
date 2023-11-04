#include "Entity.h"

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

void Entity::GetPosition(float* x, float* y)
{
    *x = m_x;
    *y = m_y;
}
