#include "FpsComponent.h"
#include "TextComponent.h"
#include <format>
#include "GameObject.h"

dae::FpsComponent::FpsComponent(GameObject* pOwner, TextComponent* pConnectedTextComponent)
    : Component(pOwner)
    , m_pConnectedTextComponent(pConnectedTextComponent)
{ }

dae::FpsComponent::FpsComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
    : Component(pOwner)
{ 
    m_pConnectedTextComponent = pOwner->AddComponent(std::make_unique<TextComponent>(pOwner, text, font, color));
}

void dae::FpsComponent::Update(float deltaTime)
{
    if (deltaTime > 0.0f && m_pConnectedTextComponent != nullptr)
    {
        float fps{ 1.f / deltaTime };
        if (abs(fps - m_prevFps) >= 0.1f) // only SetText if fps is actually different from last frame
        {
            m_pConnectedTextComponent->SetText(std::format("{:.1f}", fps) + " FPS");
            m_prevFps = fps;
        }
    }
}

std::type_index dae::FpsComponent::GetType() const
{
    return typeid(FpsComponent);
}