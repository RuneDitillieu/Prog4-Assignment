#include "MovementComponent.h"
#include "GameObject.h"
#include "DeltaTime.h"

dae::MovementComponent::MovementComponent(GameObject* pOwner, float movementSpeed)
	: Component(pOwner)
	, m_speed(movementSpeed)
{ }

void dae::MovementComponent::Update()
{
	// normalize so diagonals aren't faster than straight in 1 direction
	float length{ glm::length(m_direction) };
	if (length > 0.1f)
		m_direction /= length;

	GetOwner()->SetLocalPosition(GetOwner()->GetLocalPosition() + (m_direction * m_speed * dae::DeltaTime::GetInstance().GetDeltaTime()));

	// reset direction bc need to stop moving when button isn't pressed
	m_direction = glm::vec3(0, 0, 0);
}

void dae::MovementComponent::Move(const glm::vec3& direction)
{
	m_direction += direction;
}

std::type_index dae::MovementComponent::GetType() const
{
	return typeid(dae::MovementComponent);
}