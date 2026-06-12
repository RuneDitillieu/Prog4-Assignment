#include "RectColliderComponent.h"

dae::RectColliderComp::RectColliderComp(GameObject* pOwner, const glm::vec2& topLeft, const glm::vec2& bottomRight)
	: ColliderComp(pOwner)
	, m_topLeft(topLeft)
	, m_bottomRight(bottomRight)
{
}

bool dae::RectColliderComp::IsColliding(const glm::vec2& topLeft, const glm::vec2& bottomRight) const
{
	if (!m_isEnabled) return false;

	if (m_topLeft.x > bottomRight.x ||
		m_bottomRight.x < topLeft.x ||
		m_topLeft.y > bottomRight.y ||
		m_bottomRight.y < topLeft.y)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool dae::RectColliderComp::IsColliding(const glm::vec2& center, float radius) const
{
	if (!m_isEnabled) return false;

	float w{ m_bottomRight.x - m_topLeft.x };
	float h{ m_bottomRight.y - m_topLeft.y };

	glm::vec2 circleDistance{};

	circleDistance.x = abs(center.x - m_topLeft.x);
	circleDistance.y = abs(center.y - m_bottomRight.y);

	if (circleDistance.x > (w / 2 + radius)) { return false; }
	if (circleDistance.y > (h / 2 + radius)) { return false; }

	if (circleDistance.x <= (w / 2)) { return true; }
	if (circleDistance.y <= (h / 2)) { return true; }

	float cornerDistSq{ (circleDistance.x - w / 2) * (circleDistance.x - w / 2) +
		(circleDistance.y - h / 2) * (circleDistance.y - h / 2) };

	return (cornerDistSq <= (radius * radius));
}

std::type_index dae::RectColliderComp::GetType() const
{
	return typeid(RectColliderComp);
}