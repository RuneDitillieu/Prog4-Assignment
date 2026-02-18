#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TextComponent.h"
#include "Font.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime)
{
	if (m_textComponent != nullptr)
	{
		m_textComponent->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	if(m_texture != nullptr)
	{
		const auto& pos = m_transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}

	if (m_textComponent != nullptr)
	{
		m_textComponent->Render();
	}
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::AddTextComponent(const std::string& text, std::shared_ptr<dae::Font> font, const SDL_Color& color)
{
	m_textComponent = std::make_unique<TextComponent>(text, font, color);
}

void dae::GameObject::RemoveTextComponent()
{
	m_textComponent.reset();
}

dae::TextComponent* dae::GameObject::GetTextComponent()
{
	return m_textComponent.get();
}