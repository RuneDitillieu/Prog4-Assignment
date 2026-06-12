#include "CircleColliderComponent.h"

dae::CircleColliderComp::CircleColliderComp(GameObject* pOwner, const glm::vec2& center, float radius)
	: ColliderComp(pOwner)
	, m_collisionCenter(center)
	, m_collisionRadius(radius)
{ }

bool dae::CircleColliderComp::IsColliding(const glm::vec2& topLeft, const glm::vec2& bottomRight) const
{
	if (!m_isEnabled) return false;

	float w{ bottomRight.x - topLeft.x };
	float h{ bottomRight.y - topLeft.y };

	glm::vec2 circleDistance{};

	circleDistance.x = abs(m_collisionCenter.x - topLeft.x);
	circleDistance.y = abs(m_collisionCenter.y - bottomRight.y);

	if (circleDistance.x > (w / 2 + m_collisionRadius)) { return false; }
	if (circleDistance.y > (h / 2 + m_collisionRadius)) { return false; }

	if (circleDistance.x <= (w / 2)) { return true; }
	if (circleDistance.y <= (h / 2)) { return true; }

	float cornerDistSq{ (circleDistance.x - w / 2) * (circleDistance.x - w / 2) +
		(circleDistance.y - h / 2) * (circleDistance.y - h / 2) };

	return (cornerDistSq <= (m_collisionRadius * m_collisionRadius));
}

bool dae::CircleColliderComp::IsColliding(const glm::vec2& center, float radius) const
{
	if (!m_isEnabled) return false;

	if (glm::length(center - m_collisionCenter) <= radius + m_collisionRadius)
		return true;
	else
		return false;
}

std::type_index dae::CircleColliderComp::GetType() const
{
	return typeid(CircleColliderComp);
}