#pragma once
#include "Config.h"

class Selector
{
public:
    void Initialize();
    void Render();
    void Update(float dt, bool holding);
    void GetPosition(int* x, int* y);
    void SetPosition(int x, int y);

private:
    int m_x;
    int m_y;
    size_t m_selectorImage;
    float m_holdingElapsed;
    bool m_flashing;
};