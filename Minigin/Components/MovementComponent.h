#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;

	class MovementComponent final : public Component
	{
	public:
		MovementComponent(GameObject* pOwner, float movementSpeed = 1.f);
		~MovementComponent() = default;
		MovementComponent(const MovementComponent& other) = delete;
		MovementComponent(MovementComponent&& other) = delete;
		MovementComponent& operator=(const MovementComponent& other) = delete;
		MovementComponent& operator=(MovementComponent&& other) = delete;

		void Update() override;
		void Move(const glm::vec3& direction);

		std::type_index GetType() const override;

	private:
		float m_speed;
		glm::vec3 m_direction{ 0, 0, 0 };
	};
}