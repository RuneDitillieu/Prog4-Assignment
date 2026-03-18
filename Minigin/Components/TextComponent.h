#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "SDL3/SDL_pixels.h"
#include "Component.h"

namespace dae
{
	class Font;
	class Texture2D;
	class GameObject;
	class RenderComponent;
	class TextComponent final : public Component
	{
	public:
		TextComponent(GameObject* pOwner, RenderComponent* pConnectedRenderComponent, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
		TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
		~TextComponent();
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		// functions
		void Update(float deltaTime) override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

		std::type_index GetType() const override;

	private:
		bool m_needsUpdate{};
		std::string m_text{};
		SDL_Color m_color{ 255, 255, 255, 255 };
		std::shared_ptr<Font> m_font{};

		RenderComponent* m_pConnectedRenderComponent;
	};
}
