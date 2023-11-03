#include "GameObject.h"
#include "LevelManager.h"

void GameObject::Initialize()
{
}

void GameObject::Render()
{
}

void GameObject::Update(float dt)
{
}

void GameObject::SetGridPosition(int x, int y)
{
    m_gridX = x;
    m_gridY = y;
    LevelManager::Get().Transform(m_gridX, m_gridY, &m_pixelX, &m_pixelY);
}

void GameObject::SetPixelPosition(float x, float y)
{
    m_pixelX = x;
    m_pixelY = y;
    LevelManager::Get().Transform(m_pixelX, m_pixelY, &m_gridX, &m_gridY);
}

void GameObject::GetGridPosition(int* x, int* y)
{
    *x = m_gridX;
    *y = m_gridY;
}

void GameObject::GetPixelPosition(float* x, float* y)
{
    *x = m_pixelX;
    *y = m_pixelY;
}
