#pragma once
#include "Component.h"
#include <memory>
#include <string>
#include "SDL3/SDL_pixels.h"

namespace dae
{
    class TextComponent;
    class GameObject;
    class Font;
    class FpsComponent final : public Component
    {
    public:
        FpsComponent(GameObject* pOwner, TextComponent* pConnectedTextComponent);
        FpsComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
        ~FpsComponent() = default;
        FpsComponent(const FpsComponent& other) = delete;
        FpsComponent(FpsComponent&& other) = delete;
        FpsComponent& operator=(const FpsComponent& other) = delete;
        FpsComponent& operator=(FpsComponent&& other) = delete;

        void Update(float deltaTime) override;
        std::type_index GetType() const override;

    private:
        TextComponent* m_pConnectedTextComponent;
        float m_prevFps{ 0.f };
    };
}

