#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "SDL3/SDL_pixels.h"

namespace dae
{
	class Font;
	class TextComponent;
	class Texture2D;
	class GameObject final
	{
	public:
		virtual void Update(float deltaTime);
		virtual void Render() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		void AddTextComponent(const std::string& text, std::shared_ptr<dae::Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
		void RemoveTextComponent();
		dae::TextComponent* GetTextComponent();

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	private:
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};

		std::unique_ptr<TextComponent> m_textComponent{ nullptr };
	};
}
