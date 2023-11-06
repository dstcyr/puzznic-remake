#pragma once
#include "Entity.h"
#include "Config.h"
#include <vector>

const float INTERP_SPEED = 0.5f;

class Block : public Entity
{
public:
    void Render() override;
    void Update(float dt) override;
    void SetID(int ID);

protected:
    int m_blockID = -1;
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
};