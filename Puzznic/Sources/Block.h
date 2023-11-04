#pragma once
#include "Config.h"
#include "Animation.h"
#include "GameObject.h"
#include "Delegate.h"

struct BlockEvent : public Event
{
    BlockEvent() : tile(0), gridX(0), gridY(0)
    {
    }

    BlockEvent(int tile, int x, int y) : tile(tile), gridX(x), gridY(y)
    {
    }

    int tile = 0;
    int gridX = 0;
    int gridY = 0;
};

class Block : public GameObject
{
public:
    virtual void Initialize(int tileID);
    void Render();
    void Update(float dt) override;

    void MoveTo(int x, int y);
    void Destroy();
    void CheckFalling();
    bool IsFalling() const { return m_falling; }

    CDelegate OnBlockDestroyed;

protected:
    int m_tileID;
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
    bool m_destroyedAnimationCompleted = false;
    Animation m_blockAnimation;
    bool m_moving = false;

    virtual void UpdateInterpolation(float dt);
    void SetupInterpolation(int dx, int dy);
};