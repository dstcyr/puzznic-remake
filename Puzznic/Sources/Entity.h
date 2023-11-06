#pragma once

class Entity
{
public:
    virtual void Initialize();
    virtual void Update(float dt);
    virtual void Render();
    void SetPosition(float x, float y);
    void GetPosition(float* x, float* y);
    void GetCenter(float* x, float* y);

protected:
    float m_x = 0.0f;
    float m_y = 0.0f;
};