#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "Transform.h"
#include "SDL3/SDL_pixels.h"
#include "FpsComponent.h"
#include "Component.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final : public Component
	{
	public:
		void Update(float deltaTime) override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetPosition(float x, float y);
		void SetColor(const SDL_Color& color);

		void AddFpsComponent();
		void RemoveFpsComponent();
		std::type_index GetType() const override;

		TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
		virtual ~TextComponent();
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

	private:
		bool m_needsUpdate{};
		std::string m_text{};
		SDL_Color m_color{ 255, 255, 255, 255 };
		Transform m_transform{};
		std::shared_ptr<Font> m_font{};
		std::shared_ptr<Texture2D> m_textTexture{};

		std::unique_ptr<FpsComponent> m_fpsComponent{ nullptr };
	};
}
