#pragma once
#include "Block.h"

class Platform : public Block
{
public:
    void Update(float dt) override;
    void Render() override;
    void SetupInterpolation(int dx, int dy) override;
    void SetDirection(int dx, int dy);

private:
    int m_directionX = 0;
    int m_directionY = 0;

    void InterpolationCompleted() override;
};