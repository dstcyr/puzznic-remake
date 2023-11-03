#pragma once
#include "Config.h"
#include "Animation.h"

class Block
{
public:
    void Initialize(int tileID);
    void Render();
    bool Update(float dt);
    void GetPosition(int* x, int* y);
    void SetPosition(int x, int y);
    void MoveTo(int x, int y);
    void Destroy();
    void CheckFalling();
    bool IsFalling() const { return m_falling; }

private:
    int m_tileID;
    float m_worldX = 0.0f;
    float m_worldY = 0.0f;
    int m_localX = 0;
    int m_localY = 0;

    float m_interpTime = 0.0f;
    float m_interpSpeed = 0.2f;
    float m_interpStartX = 0.0f;
    float m_interpStartY = 0.0f;
    float m_interpEndX = 0.0f;
    float m_interpEndY = 0.0f;
    int m_interpLocalX = 0;
    int m_interpLocalY = 0;
    bool m_falling;
    bool m_destroyed;

    Animation m_blockAnimation;

    void UpdateInterpolation(float dt);
    void SetupInterpolation(int dx, int dy);

};