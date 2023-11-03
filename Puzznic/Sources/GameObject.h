#pragma once
#include "Config.h"

class GameObject
{
public:
    void Initialize();
    void Render();
    virtual void Update(float dt);
    void SetGridPosition(int x, int y);
    void SetPixelPosition(float x, float y);
    void GetGridPosition(int* x, int* y);
    void GetPixelPosition(float* x, float* y);

protected:
    float m_pixelX = 0.0f;
    float m_pixelY = 0.0f;
    int m_gridX = 0;
    int m_gridY = 0;
};