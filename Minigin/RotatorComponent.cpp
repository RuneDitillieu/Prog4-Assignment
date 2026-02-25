#include "RotatorComponent.h"
#include "GameObject.h"

dae::RotatorComponent::RotatorComponent(GameObject* pOwner, const glm::vec3& pivot, float clockwiseDirection)
	: Component(pOwner)
	, m_pivot(pivot), m_clockwiseDirection(clockwiseDirection), m_distance(static_cast<float>(glm::length((GetOwner()->GetLocalPosition() - m_pivot))))
{
}

void dae::RotatorComponent::Update(float deltaTime)
{
	m_angle += deltaTime * m_clockwiseDirection;
	float x{ cos(m_angle) * m_distance };
	float y{ sin(m_angle) * m_distance };
	glm::vec3 newPos{ m_pivot + glm::vec3{ x, y, 0 } };
	GetOwner()->SetLocalPosition(newPos);
}

std::type_index dae::RotatorComponent::GetType() const
{
	return typeid(RotatorComponent);
}