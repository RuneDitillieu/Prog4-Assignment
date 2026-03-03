#include "RotatorComponent.h"
#include "GameObject.h"

dae::RotatorComponent::RotatorComponent(GameObject* pOwner, float clockwiseSpeed)
	: Component(pOwner)
	, m_clockwiseSpeed(clockwiseSpeed), m_distance(static_cast<float>(glm::length((GetOwner()->GetLocalPosition()))))
{
}

void dae::RotatorComponent::Update(float deltaTime)
{
	m_angle += deltaTime * m_clockwiseSpeed;

	if (m_angle > 360)
	{
		m_angle -= 360.f;
	}
	else if (m_angle < -360)
	{
		m_angle += 360.f;
	}

	float x{ cos(m_angle) * m_distance };
	float y{ sin(m_angle) * m_distance };
	glm::vec3 newPos{ glm::vec3{ x, y, 0 } };
	GetOwner()->SetLocalPosition(newPos);
}

std::type_index dae::RotatorComponent::GetType() const
{
	return typeid(RotatorComponent);
}