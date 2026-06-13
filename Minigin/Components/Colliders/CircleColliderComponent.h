#ifndef CIRCLECOLLIDERCOMPONENT_H
#define CIRCLECOLLIDERCOMPONENT_H

#include "ColliderComponent.h"

namespace dae
{
	class GameObject;
	class CircleColliderComp : public ColliderComp
	{
	public:
		explicit CircleColliderComp(GameObject* pOwner, const glm::vec2& center, float radius);
		~CircleColliderComp() = default;
		CircleColliderComp(const CircleColliderComp& other) = delete;
		CircleColliderComp(CircleColliderComp&& other) = delete;
		CircleColliderComp& operator=(const CircleColliderComp& other) = delete;
		CircleColliderComp& operator=(CircleColliderComp&& other) = delete;

		bool IsColliding(const glm::vec2& topLeft, const glm::vec2& bottomRight) const override;
		bool IsColliding(const glm::vec2& center, float radius) const override;

		std::type_index GetType() const override;

	private:
		glm::vec2 m_collisionCenter;
		float m_collisionRadius;
	};
}

#endif // !CIRCLECOLLIDERCOMPONENT_H