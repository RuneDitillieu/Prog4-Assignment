#ifndef RECTCOLLIDERCOMPONENT_H
#define RECTCOLLIDERCOMPONENT_H

#include "ColliderComponent.h"

namespace dae
{
	class GameObject;
	class RectColliderComp : public ColliderComp
	{
	public:
		RectColliderComp(dae::GameObject* pOwner, const glm::vec2& topLeft, const glm::vec2& bottomRight);
		~RectColliderComp() = default;
		RectColliderComp(const RectColliderComp& other) = delete;
		RectColliderComp(RectColliderComp&& other) = delete;
		RectColliderComp& operator=(const RectColliderComp& other) = delete;
		RectColliderComp& operator=(RectColliderComp&& other) = delete;

		bool IsColliding(const glm::vec2& topLeft, const glm::vec2& bottomRight) const override;
		bool IsColliding(const glm::vec2& center, float radius) const override;

		std::type_index GetType() const override;

	private:
		glm::vec2 m_topLeft;
		glm::vec2 m_bottomRight;
	};
}

#endif // !RECTCOLLIDERCOMPONENT_H
