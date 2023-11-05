#pragma once
#include "Block.h"
#include "Animation.h"

class AnimatedBlock : public Block
{
public:
    void Initialize() override;
    void Update(float dt) override;
    void Render() override;
    void UpdateInterpolation(float dt) override;

protected:

private:
    Animation m_blockAnimation;
};