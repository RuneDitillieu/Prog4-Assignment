#include "FpsComponent.h"
#include "TextComponent.h"
#include <format>
#include "GameObject.h"

dae::FpsComponent::FpsComponent(GameObject* pOwner, TextComponent* pConnectedTextComponent)
    : Component(pOwner)
    , m_pConnectedTextComponent(pConnectedTextComponent)
{
    m_prev10FramesFps.reserve(30);
}

dae::FpsComponent::FpsComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
    : Component(pOwner)
{ 
    m_pConnectedTextComponent = pOwner->AddComponent(std::make_unique<TextComponent>(pOwner, text, font, color));
    m_prev10FramesFps.reserve(30);
}

void dae::FpsComponent::Update(float deltaTime)
{
    if (deltaTime > 0.001f && m_pConnectedTextComponent != nullptr)
    {
        if (m_prev10FramesFps.size() == m_prev10FramesFps.capacity())
        {
            float avgFps{};
            float smallestResult{ m_prev10FramesFps[0] };
            float biggestResult{ };
            for (float fps : m_prev10FramesFps)
            {
                avgFps += fps;
                if (fps > biggestResult)
                    biggestResult = fps;
                if (fps < smallestResult)
                    smallestResult = fps;
            }
            avgFps -= smallestResult;
            avgFps -= biggestResult;
            avgFps /= (m_prev10FramesFps.size() - 2);

            m_pConnectedTextComponent->SetText(std::format("{:.1f}", avgFps) + " FPS");
            m_prev10FramesFps.clear();
        }
        else
        {
            m_prev10FramesFps.emplace_back(1.f / deltaTime);
        }

        /*m_timer += deltaTime;
        ++m_frameCount;

        if (m_timer >= 0.5f)
        {
            float fps{ static_cast<float>(m_frameCount / m_timer) };

            if (std::abs(fps - m_prevFps) > 0.1f)
            {
                m_pConnectedTextComponent->SetText(std::format("{:.1f} FPS", fps));
                m_prevFps = fps;
            }

            m_timer = 0.0f;
            m_frameCount = 0;
        }*/
    }
}

std::type_index dae::FpsComponent::GetType() const
{
    return typeid(FpsComponent);
}