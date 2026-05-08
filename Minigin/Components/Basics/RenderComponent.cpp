#include "RenderComponent.h"
#include "Renderer.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Texture2D.h"

dae::RenderComponent::RenderComponent(GameObject* pOwner, const std::string& filename)
	: Component(pOwner)
{ 
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

dae::RenderComponent::RenderComponent(GameObject* pOwner)
	: Component(pOwner)
{
	m_texture = nullptr;
}

dae::RenderComponent::~RenderComponent() = default;

void dae::RenderComponent::Render(const Transform& transform)
{
	if (m_texture != nullptr)
	{
		const auto& pos = transform.GetPosition();

		if (m_useSourceRect)
		{
			SDL_FRect dstRect{ pos.x, pos.y,
			m_sourceRect.w * transform.GetScale(), m_sourceRect.h * transform.GetScale() };
			Renderer::GetInstance().RenderTexture(*m_texture, m_sourceRect, dstRect);
		}
		else
		{
			SDL_FRect dstRect{ pos.x, pos.y,
			m_texture->GetSize().x * transform.GetScale(), m_texture->GetSize().y * transform.GetScale() };
			Renderer::GetInstance().RenderTexture(*m_texture, dstRect);
		}
	}
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetTexture(std::shared_ptr<Texture2D>&& texture)
{
	m_texture = texture;
}

void dae::RenderComponent::SetSourceRect(const SDL_FRect& src)
{
	if (src.w != 0 && src.h != 0)
		m_useSourceRect = true;
	else
		m_useSourceRect = false;

	m_sourceRect = src;
}

void dae::RenderComponent::SetSourceRect(float x, float y, float width, float height)
{
	SDL_FRect srcRect{ x, y, width, height };
	SetSourceRect(srcRect);
}

glm::vec2 dae::RenderComponent::GetSize() const
{
	return m_texture->GetSize();
}

std::type_index dae::RenderComponent::GetType() const
{
	return typeid(RenderComponent);
}