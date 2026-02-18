#include "FpsComponent.h"

dae::FpsComponent::FpsComponent()
{
}

float dae::FpsComponent::GetCurrentFps(float deltaTime)
{
    if (deltaTime <= 0.0f) return 0.f;

    return static_cast<float>(1.f / deltaTime);
}