#include "GameObject.h"
#include "Component.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime)
{
	for (auto& component : m_components)
	{
		component->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	for (auto& component : m_components)
	{
		component->Render(m_transform);
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}