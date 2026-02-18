#include "RenderComponent.h"
#include "Renderer.h"
#include "Transform.h"
#include "ResourceManager.h"

dae::RenderComponent::RenderComponent(GameObject* owner, const std::string& filename)
	: Component(owner)
{ 
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

dae::RenderComponent::~RenderComponent() = default;

void dae::RenderComponent::Render(const Transform& transform) const
{
	if (m_texture != nullptr)
	{
		const auto& pos = transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

std::type_index dae::RenderComponent::GetType() const
{
	return typeid(RenderComponent);
}