#pragma once
#include "Block.h"
#include "Animation.h"

class AnimatedBlock : public Block
{
public:
    void Initialize() override;
    void Update(float dt) override;
    void Render() override;

protected:
    void UpdateInterpolation(float dt) override;

private:
    Animation m_blockAnimation;
};