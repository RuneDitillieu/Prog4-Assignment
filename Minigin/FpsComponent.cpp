#include "FpsComponent.h"
#include "TextComponent.h"
#include <format>

dae::FpsComponent::FpsComponent(TextComponent* owner)
    : m_owner(owner)
{ }

void dae::FpsComponent::Update(float deltaTime)
{
    if (deltaTime > 0.0f)
    {
        float fps{ 1.f / deltaTime };
        m_owner->SetText(std::format("{:.1f}", fps) + " FPS");
    }
}