#pragma once
#include "Entity.h"
#include "Config.h"
#include <vector>
#include "Delegate.h"

const float INTERP_SPEED = 0.5f;

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
    float current = 0.0f;
    int local = 0;
    bool moving = false;
};

class Block : public Entity
{
public:
    void Initialize() override;

    void Render() override;
    void Update(float dt) override;
    void SetID(int ID);



    bool InstantMove(int dx, int dy);


    CDelegate OnBlockInstantMove;

    void GetDestination(int* dx, int* dy);

    bool MoveHorizontal(int direction);
    bool MoveVertical(int direction);
    void Update(float dt, BlockInterpolation& interp);

    bool IsMoving();

protected:
    int m_blockID = -1;

    BlockInterpolation m_horizontalMovements;
    BlockInterpolation m_verticalMovements;

    void InitInterp(BlockInterpolation& interp, float start, float end);
    void GetDestination(int dirX, int dirY, float* endX, float* endY);
    bool HitWall(float x, float y);
    bool HitBlock(float x, float y);


private:
};

class DebugBlock : public Block
{
public:
    void Render() override;
    void Update(float dt) override;
};

class Platform : public Block
{
public:
    void Render() override;
    void Update(float dt) override;
    void Start(bool horizontal);

    void OnBlockInstantMove(const BlockEvent& e);

private:
    int m_direction = 1;
    bool m_movingHorizontal = false;

    std::vector<Block*> m_childBlocks;
};