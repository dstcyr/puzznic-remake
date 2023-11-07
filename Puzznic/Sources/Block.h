#pragma once
#include "Entity.h"
#include "Config.h"
#include <vector>
#include "Delegate.h"

const float INTERP_SPEED = 0.2f;

class Block;

struct BlockEvent : public Event
{
    BlockEvent() : BlockEvent(nullptr) {}
    BlockEvent(Block* block) : block(block) {}
    Block* block;
};

struct BlockInterpolation
{
    float time = 0.0f;
    float speed = 0.2f;
    float start = 0.0f;
    float end = 0.0f;
    bool moving = false;
};

class Block : public Entity
{
public:
    void Render() override;
    void Update(float dt) override;
    void SetID(int ID);

protected:
    int m_blockID = -1;


    bool CanFall();
};

class PlacedBlock : public Block
{
public:
    void Render() override;
    void Update(float dt) override;
    void Fall(float dt);
    void Teleport(int direction);

    CDelegate OnBlockTeleported;

protected:
    BlockInterpolation m_fallingInterp;
};

class Platform : public Block
{
public:
    void Render() override;
    void Update(float dt) override;

protected:
    BlockInterpolation m_Interp;
    bool m_inPause = false;
    float m_pauseElapsed = 0.0f;
    int m_direction = 1;

    virtual void StartMoving();
    void Pause();
    void UpdatePause(float dt);
    virtual void InterpolationEnd(int dx, int dy);
    void UpdateInterp(float* currentValue, int dx, int dy, float dt);

};

class HorizontalPlatform : public Platform
{
public:
    void Update(float dt) override;

protected:
    void StartMoving() override;
};

class VerticalPlatform : public Platform
{
public:
    void Update(float dt) override;

private:
    void StartMoving() override;
};