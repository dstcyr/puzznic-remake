#pragma once
#include "Entity.h"
#include "Config.h"

class Block : public Entity
{
public:
    void Render() override;
    void SetID(int ID);
    virtual void SetupInterpolation(int dx, int dy);
    bool IsMoving() const { return m_moving; }

protected:
    float m_interpTime = 0.0f;
    float m_interpSpeed = INTERP_SPEED;
    float m_interpStartX = 0.0f;
    float m_interpStartY = 0.0f;
    float m_interpEndX = 0.0f;
    float m_interpEndY = 0.0f;
    int m_interpTargetX = 0;
    int m_interpTargetY = 0;
    int m_interpSourceX = 0;
    int m_interpSourceY = 0;
    int m_interpDirX = 0;
    int m_interpDirY = 0;
    bool m_moving = false;
    int m_blockID = -1;

    virtual void UpdateInterpolation(float dt);
    virtual void InterpolationCompleted();
};