#pragma once
#include "Config.h"
#include "GameObject.h"

class Selector : public GameObject
{
public:
    void Initialize();
    void Render();
    void Update(float dt) override;
    void ResetFlash();

private:
    size_t m_selectorImage = 0;
    float m_holdingElapsed = 0.0f;
    bool m_flashing = false;
};