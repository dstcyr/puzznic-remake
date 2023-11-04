#pragma once
#include "Block.h"

class MovingBlock : public Block
{
public:
    void Initialize(int tileID) override;
    void Update(float dt) override;

protected:
    void UpdateInterpolation(float dt) override;

private:
    int m_dirX = 0;
    int m_dirY = 0;
    bool CanMove();
};