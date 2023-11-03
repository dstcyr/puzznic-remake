#pragma once
#include "Config.h"

class Selector
{
public:
    void Initialize();
    void Render();
    void Update(float dt, bool holding);
    void GetPosition(int* x, int* y);


    void SetGridPosition(int x, int y);
    void SetPixelPosition(float x, float y);




private:
    int m_x;
    int m_y;
    float m_worldX;
    float m_worldY;
    size_t m_selectorImage;
    float m_holdingElapsed;
    bool m_flashing;
};