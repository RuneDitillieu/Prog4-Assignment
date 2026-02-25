#include "FpsComponent.h"
#include "TextComponent.h"
#include <format>
#include "GameObject.h"

dae::FpsComponent::FpsComponent(GameObject* pOwner, TextComponent* pConnectedTextComponent)
    : Component(pOwner)
    , m_pConnectedTextComponent(pConnectedTextComponent)
{
    m_prev10FramesFps.reserve(10);
}

dae::FpsComponent::FpsComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
    : Component(pOwner)
{ 
    m_pConnectedTextComponent = pOwner->AddComponent(std::make_unique<TextComponent>(pOwner, text, font, color));
    m_prev10FramesFps.reserve(10);
}

void dae::FpsComponent::Update(float deltaTime)
{
    if (deltaTime > 0.001f && m_pConnectedTextComponent != nullptr)
    {
        if (m_prev10FramesFps.size() == m_prev10FramesFps.capacity())
        {
            float avgFps{};
            for (float fps : m_prev10FramesFps)
            {
                avgFps += fps;
            }
            avgFps /= m_prev10FramesFps.size();

            m_pConnectedTextComponent->SetText(std::format("{:.1f}", avgFps) + " FPS");
            m_prev10FramesFps.clear();
        }
        else
        {
            m_prev10FramesFps.emplace_back(1.f / deltaTime);
        }
    }
}

std::type_index dae::FpsComponent::GetType() const
{
    return typeid(FpsComponent);
}