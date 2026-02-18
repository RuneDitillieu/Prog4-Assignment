#include "Component.h"

dae::Component::Component(GameObject* owner)
	: m_owner(owner)
{}

void dae::Component::Update(float deltaTime)
{
	deltaTime;
}

void dae::Component::Render(const Transform& transform) const
{
	transform;
}